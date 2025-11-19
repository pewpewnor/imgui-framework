#include <chrono>
#include <cstring>
#include <future>
#include <optional>
#include <string>

#include "result.h"

template <typename TResult>
class AsyncWorker {
public:
    AsyncWorker<TResult>(bool invalidateOldCache = false)
        : invalidateOldCache_(invalidateOldCache) {}

    /**
     *  @brief spawning another while the previous future is still working
     *  will block the main thread (since the old future's destructor is called)
     */
    template <typename TFunc, typename... TArgs>
    void spawn(TFunc&& func, TArgs&&... args) {
        if (invalidateOldCache_) {
            cachedResult_.reset();
        }
        future_ =
            std::async(std::launch::async, std::forward<TFunc>(func), std::forward<TArgs>(args)...);
    }

    [[nodiscard]] bool hasResult() const {
        if (cachedResult_.has_value()) {
            return true;
        }
        return isDoneWorking();
    }

    [[nodiscard]] bool isBusyWorking() const { return future_.valid(); }

    [[nodiscard]] Result<std::string> getResultBlocking() {
        if (!invalidateOldCache_ && isDoneWorking()) {
            return result();
        }
        if (cachedResult_.has_value()) {
            return cachedResult_.value();
        }
        if (!future_.valid()) {
            return std::unexpected("no valid async task exists to retrieve");
        }
        return result();
    }

    [[nodiscard]] Fallible waitUntilFinished() {
        if (!future_.valid()) {
            return {};
        }
        try {
            future_.get();
            return {};
        } catch (const std::exception& e) {
            return e.what();
        } catch (...) {
            return "unknown async error occurred";
        }
    }

private:
    bool invalidateOldCache_ = true;
    std::optional<TResult> cachedResult_;
    std::future<TResult> future_;

    [[nodiscard]] bool isDoneWorking() const {
        return future_.valid() &&
               future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    [[nodiscard]] Result<std::string> result() {
        try {
            cachedResult_ = std::move(future_.get());
            return cachedResult_.value();
        } catch (const std::exception& e) {
            return std::unexpected(e.what());
        } catch (...) {
            return std::unexpected("unknown async error occurred");
        }
    }
};

template <>
class AsyncWorker<void> {
public:
    /**
     *  @brief spawning another while the previous future is still working
     *  will block the main thread (since the old future's destructor is called)
     */
    template <typename TFunc, typename... TArgs>
    void spawn(TFunc&& func, TArgs&&... args) {
        future_ =
            std::async(std::launch::async, std::forward<TFunc>(func), std::forward<TArgs>(args)...);
    }

    [[nodiscard]] bool hasResult() const {
        if (!future_.valid()) {
            return false;
        }
        return future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    [[nodiscard]] bool isBusyWorking() const { return future_.valid(); }

    [[nodiscard]] Fallible getResultBlocking() {
        if (!future_.valid()) {
            return "no valid async task exists to retrieve";
        }
        try {
            future_.get();
            return {};
        } catch (const std::exception& e) {
            return e.what();
        } catch (...) {
            return "unknown async error occurred";
        }
    }

    [[nodiscard]] Fallible waitUntilFinished() { return getResultBlocking(); }

private:
    std::future<void> future_;
};
