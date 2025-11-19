#include "workers.h"

#include "spdlog/spdlog.h"
#include "utils/result.h"

Workers::~Workers() {
    spdlog::info("Waiting for remaining async workers to finish...");
    if (Fallible error = sleepWorker.waitUntilFinished()) {
        spdlog::error("Failed to kill sleep worker: {}", error.value());
    }
}
