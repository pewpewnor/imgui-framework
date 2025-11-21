#include "ignored_futures.h"

#include "spdlog/spdlog.h"

namespace globals {

std::unique_ptr<globals::IgnoredFutures> ignoredFutures;

}

globals::IgnoredFutures::~IgnoredFutures() {
    spdlog::debug("Waiting for all ignored futures to finish...");
    std::lock_guard<std::mutex> lock(mutex);
    for (const std::shared_future<void>& future : futures) {
        future.wait_for(std::chrono::seconds(10));
    }
}
