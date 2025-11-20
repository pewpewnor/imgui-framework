#include "ignored_tasks.h"

#include "spdlog/spdlog.h"

void ignored_tasks::waitAllIgnoredFutures(std::chrono::seconds waitTimeSeconds) {
    spdlog::debug("Waiting for all ignored futures to finish...");
    std::lock_guard<std::mutex> lock(ignored_tasks::ignoredFuturesMutex);
    for (const std::shared_future<void>& future : ignored_tasks::ignoredFutures) {
        future.wait_for(waitTimeSeconds);
    }
}

void ignored_tasks::clearAllIgnoredFutures() {
    spdlog::debug("Waiting for all ignored futures to finish...");
    std::lock_guard<std::mutex> lock(ignored_tasks::ignoredFuturesMutex);
    ignored_tasks::ignoredFutures.clear();
}
