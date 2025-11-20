#pragma once

#include <string>
#include <string_view>

#include "common/engine_state.h"
#include "common/ignored_tasks.h"
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
            spdlog::debug("<{}> Ignored a task...", taskName);
            std::lock_guard<std::mutex> lock(ignored_tasks::ignoredFuturesMutex);
            ignored_tasks::ignoredFutures.push_back(std::move(this->future));
        }
    }

protected:
    std::string taskName;

    void runTask(TaskFunction<TResult> task) {
        std::string taskNameCapture = taskName;

        auto onSuccess = [taskNameCapture](const TResult&) {
            spdlog::debug("<{}> Task complete, sending refresh signal...", taskNameCapture);
            globals::engine->sendRefreshSignal();
        };
        auto onFailure = [taskNameCapture](std::string_view errorMsg) {
            spdlog::error("<{}> Error detected: {}", taskNameCapture, errorMsg);
        };

        this->spawn(std::move(task), std::move(onSuccess), std::move(onFailure));
    }
};
