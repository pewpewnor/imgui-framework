#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <tuple>

#include "assertion.h"
#include "result.h"

template <typename TResult>
class AsyncWorker {
public:
    AsyncWorker(bool invalidateOldCache = false) : invalidateOldCache_(invalidateOldCache) {}

    AsyncWorker(const AsyncWorker&) = delete;
    AsyncWorker(AsyncWorker&&) = delete;
    AsyncWorker& operator=(const AsyncWorker&) = delete;
    AsyncWorker& operator=(AsyncWorker&&) = delete;

    ~AsyncWorker() {
        if (future_.valid()) {
            future_.wait();
        }
    }

    [[nodiscard]] bool isFreeToSpawn() const { return !future_.valid(); }

    template <typename TTaskFunc, typename TPostFunc = std::function<void()>, typename... TArgs>
    Fallible spawnBlocking(TTaskFunc&& task, TPostFunc&& postTask, TArgs&&... taskArgs) {
        ASSERT_HARD(isFreeToSpawn(),
                    "Previous future task must be complete/consumed to spawn a new task");

        if (invalidateOldCache_) {
            std::lock_guard<std::mutex> lock(cacheMutex_);
            cachedResult_.reset();
        }

        working_ = true;

        future_ =
            std::async(std::launch::async,
                       [argsTuple = std::make_tuple(std::forward<TArgs>(taskArgs)...),
                        task = std::forward<TTaskFunc>(task),
                        postTask = std::forward<TPostFunc>(postTask), this]() mutable -> void {
                           auto result = std::apply(
                               [&task](auto&&... unpackedArgs) {
                                   return std::invoke(
                                       task, std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
                               },
                               std::move(argsTuple));
                           {
                               std::lock_guard<std::mutex> lock(this->cacheMutex_);
                               this->cachedResult_ = std::move(result);
                           }
                           this->working_ = false;
                           postTask();
                       });
        return {};
    }

    template <typename TFunc, typename... TArgs>
    Fallible spawnBlocking(TFunc&& task, TArgs&&... taskArgs) {
        return spawnBlocking(std::forward<TFunc>(task), []() {}, std::forward<TArgs>(taskArgs)...);
    }

    [[nodiscard]] bool hasResult() const {
        {
            std::lock_guard<std::mutex> lock(cacheMutex_);
            return cachedResult_.has_value();
        }
        return future_.valid() &&
               future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    [[nodiscard]] bool isBusyWorking() const { return working_; }

    [[nodiscard]] Result<TResult> getResultBlocking() {
        {
            std::lock_guard<std::mutex> lock(cacheMutex_);
            if (!future_.valid() && cachedResult_.has_value()) {
                return cachedResult_.value();
            }
        }
        if (!future_.valid()) {
            return std::unexpected("No async task active and no cache available");
        }
        if (Fallible error = get()) {
            return std::unexpected(error.value());
        }
        std::lock_guard<std::mutex> lock(cacheMutex_);
        ASSERT_HARD(cachedResult_.has_value(), "Finished task must set a value to result");
        return cachedResult_.value();
    }

    [[nodiscard]] Fallible waitUntilFinished() {
        if (!future_.valid()) {
            return {};
        }
        return get();
    }

private:
    bool invalidateOldCache_;
    mutable std::mutex cacheMutex_;
    std::optional<TResult> cachedResult_;
    std::atomic<bool> working_ = false;
    std::future<void> future_;

    [[nodiscard]] Fallible get() {
        try {
            if (future_.valid()) {
                future_.get();
            }
            return {};
        } catch (const std::exception& e) {
            return e.what();
        } catch (...) {
            return "Unknown async error occurred";
        }
    }
};
