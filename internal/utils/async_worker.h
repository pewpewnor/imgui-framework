#pragma once

#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "assertions.h"
#include "result.h"

template <typename TResult>
using TaskFunction = std::function<TResult()>;
template <typename TResult>
using SuccessCallback = std::function<void(const TResult& result)>;
using FailureCallback = std::function<void(std::string_view errorMsg)>;

template <typename TResult>
class AsyncWorker {
public:
    std::shared_future<void> future;

    AsyncWorker(bool invalidateOldCache = false)
        : outcome_(std::make_shared<TaskOutcome>()), invalidateOldCache_(invalidateOldCache) {}

    AsyncWorker(AsyncWorker&&) = delete;
    AsyncWorker& operator=(AsyncWorker&&) = delete;
    AsyncWorker(const AsyncWorker&) = delete;
    AsyncWorker& operator=(const AsyncWorker&) = delete;
    virtual ~AsyncWorker() = default;

    [[nodiscard]] bool isAvailable() const { return !isBusy(); }

    [[nodiscard]] bool isBusy() const {
        return future.valid() &&
               future.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }

    [[nodiscard]] bool hasResult() const {
        std::lock_guard<std::mutex> lock(outcome_->mutex);
        return outcome_->result.has_value();
    }

    Result<TResult> getResult() {
        std::lock_guard<std::mutex> lock(outcome_->mutex);
        if (outcome_->error.has_value()) {
            std::string errorMessage = outcome_->error.value();
            outcome_->error.reset();
            return std::unexpected{errorMessage};
        }
        ASSERT(outcome_->result.has_value(), "task result or error must be available to retrieve");
        return outcome_->result.value();
    }

protected:
    void spawnTaskWithCallbacks(TaskFunction<TResult> task, SuccessCallback<TResult> onSuccess,
                                FailureCallback onFailure) {
        ASSERT(isAvailable(), "must be available to spawn a new task");

        std::shared_ptr<TaskOutcome> prevCore = outcome_;
        outcome_ = std::make_shared<TaskOutcome>();
        if (!invalidateOldCache_ && prevCore) {
            std::lock_guard<std::mutex> lockPrevCore(prevCore->mutex);
            std::lock_guard<std::mutex> lockCore(outcome_->mutex);
            outcome_->result = prevCore->result;
        }

        future = std::async(std::launch::async, [outcome = this->outcome_, task = std::move(task),
                                                 onSuccess = std::move(onSuccess),
                                                 onFailure = std::move(onFailure)]() {
                     try {
                         TResult res = task();
                         {
                             std::lock_guard<std::mutex> lock(outcome->mutex);
                             outcome->result = res;
                         }
                         if (onSuccess) {
                             onSuccess(res);
                         }
                     } catch (const std::exception& e) {
                         {
                             std::lock_guard<std::mutex> lock(outcome->mutex);
                             outcome->error = e.what();
                         }
                         if (onFailure) {
                             onFailure(e.what());
                         }
                     } catch (...) {
                         std::string unknownMsg = "unknown async task error";
                         {
                             std::lock_guard<std::mutex> lock(outcome->mutex);
                             outcome->error = unknownMsg;
                         }
                         if (onFailure) {
                             onFailure(unknownMsg);
                         }
                     }
                 }).share();
    }

private:
    struct TaskOutcome {
        std::mutex mutex;
        Fallible error;
        std::optional<TResult> result;
    };

    std::shared_ptr<TaskOutcome> outcome_;
    bool invalidateOldCache_;
};
