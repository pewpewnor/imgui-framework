#pragma once

#include <string>
#include <string_view>

#include "engine_state.h"
#include "ignored_tasks.h"
#include "spdlog/spdlog.h"
#include "utils/async_task.h"

template <typename TResult>
class Task : public virtual AsyncTask<TResult> {
public:
    Task(std::string_view taskName) : taskName(taskName) {}

    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;
    virtual ~Task() = default;

    void ignore() {
        if (this->isBusy()) {
            std::lock_guard<std::mutex> lock(ignored_tasks::ignoredFuturesMutex);
            ignored_tasks::ignoredFutures.push_back(std::move(this->future));
            spdlog::debug("<{}> Task ignored", taskName);
        }
    }

protected:
    std::string taskName;

    void runTask(typename AsyncTask<TResult>::TaskFunction task) {
        std::string taskNameCapture = taskName;

        auto onSuccess = [taskNameCapture](const TResult&) {
            spdlog::debug("<{}> Task complete. Sending refresh signal...", taskNameCapture);
            globals::engine->sendRefreshSignal();
        };
        auto onFailure = [taskNameCapture](std::string_view errorMsg) {
            spdlog::error("<{}> Error detected: {}", taskNameCapture, errorMsg);
        };

        this->spawn(std::move(task), std::move(onSuccess), std::move(onFailure));
    }
};
