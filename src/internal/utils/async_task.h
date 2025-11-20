#pragma once

#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "assertion.h"
#include "result.h"

template <typename TResult>
class AsyncTask {
public:
    using TaskFunction = std::function<TResult()>;
    using SuccessCallback = std::function<void(const TResult& result)>;
    using FailureCallback = std::function<void(std::string_view errorMsg)>;

    std::shared_future<void> future;

    AsyncTask(bool invalidateOldCache = false)
        : core_(std::make_shared<TaskCore>()), invalidateOldCache_(invalidateOldCache) {}

    AsyncTask(AsyncTask&&) = delete;
    AsyncTask& operator=(AsyncTask&&) = delete;
    AsyncTask(const AsyncTask&) = delete;
    AsyncTask& operator=(const AsyncTask&) = delete;
    virtual ~AsyncTask() = default;

    [[nodiscard]] bool isAvailable() const { return !isBusy(); }

    [[nodiscard]] bool isBusy() const {
        return future.valid() &&
               future.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }

    [[nodiscard]] bool hasResult() const {
        std::lock_guard<std::mutex> lock(core_->outcomeMutex);
        return core_->result.has_value();
    }

    Result<TResult> getResult() {
        std::lock_guard<std::mutex> lock(core_->outcomeMutex);
        if (core_->error.has_value()) {
            std::string errorMessage = core_->error.value();
            core_->error.reset();
            return std::unexpected{errorMessage};
        }
        ASSERT_HARD(core_->result.has_value(), "result must be available to retrieve");
        return core_->result.value();
    }

protected:
    void spawn(TaskFunction task, SuccessCallback onSuccess, FailureCallback onFailure) {
        ASSERT_HARD(isAvailable(), "must be available to spawn a new task");
        {
            std::lock_guard<std::mutex> lock(core_->outcomeMutex);
            if (invalidateOldCache_) {
                core_->result.reset();
            }
            core_->error.reset();
        }
        std::shared_ptr<TaskCore> coreCapture = core_;

        future = std::async(std::launch::async, [coreCapture, task = std::move(task),
                                                 onSuccess = std::move(onSuccess),
                                                 onFailure = std::move(onFailure)]() {
                     try {
                         TResult res = task();
                         {
                             std::lock_guard<std::mutex> lock(coreCapture->outcomeMutex);
                             coreCapture->result = res;
                         }
                         if (onSuccess) {
                             onSuccess(res);
                         }
                     } catch (const std::exception& e) {
                         {
                             std::lock_guard<std::mutex> lock(coreCapture->outcomeMutex);
                             coreCapture->error = e.what();
                         }
                         if (onFailure) {
                             onFailure(e.what());
                         }
                     } catch (...) {
                         std::string unknownMsg = "unknown async task error";
                         {
                             std::lock_guard<std::mutex> lock(coreCapture->outcomeMutex);
                             coreCapture->error = unknownMsg;
                         }
                         if (onFailure) {
                             onFailure(unknownMsg);
                         }
                     }
                 }).share();
    }

    std::string getErrorMessage() {
        if (!core_) {
            return "No task core";
        }
        std::lock_guard<std::mutex> lock(core_->outcomeMutex);
        return core_->error.value_or("");
    }

private:
    struct TaskCore {
        std::mutex outcomeMutex;
        Fallible error;
        std::optional<TResult> result;
    };

    std::shared_ptr<TaskCore> core_;
    bool invalidateOldCache_;
};
