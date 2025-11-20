#pragma once

#include <future>
#include <mutex>
#include <optional>

#include "assertion.h"

template <typename TResult>
class AsyncTask {
public:
    AsyncTask(bool invalidateOldCache = false) : invalidateOldCache_(invalidateOldCache) {}

    AsyncTask(AsyncTask&&) = delete;
    AsyncTask& operator=(AsyncTask&&) = delete;
    AsyncTask(const AsyncTask&) = delete;
    AsyncTask& operator=(const AsyncTask&) = delete;

    virtual ~AsyncTask() {
        if (future_.valid()) {
            future_.wait();
        }
    }

    [[nodiscard]] bool isBusy() const {
        return future_.valid() &&
               future_.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }

    [[nodiscard]] bool hasResult() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return result_.has_value();
    }

    TResult getResult() {
        std::lock_guard<std::mutex> lock(mutex_);
        ASSERT_HARD(result_.has_value(), "result must be available to retrieve");
        return result_.value();
    }

protected:
    virtual void execute() = 0;

    virtual void postTask() {}

    void spawn() {
        ASSERT_HARD(!isBusy(), "must not be busy to run new task");
        if (invalidateOldCache_) {
            std::lock_guard<std::mutex> lock(mutex_);
            result_.reset();
        }
        future_ = std::async(std::launch::async, [this]() {
            this->execute();
            this->postTask();
        });
    }

    void submitResult(TResult&& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        result_ = std::move(value);
    }

private:
    bool invalidateOldCache_;
    mutable std::mutex mutex_;
    std::optional<TResult> result_;
    std::future<void> future_;
};
