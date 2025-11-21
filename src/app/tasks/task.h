#pragma once

#include <string>
#include <string_view>

#include "common/engine_state.h"
#include "common/ignored_tasks.h"
#include "spdlog/spdlog.h"
#include "utils/assertions.h"
#include "utils/async_task.h"

template <typename TResult>
class Task : public AsyncTask<TResult> {
public:
    Task() = default;
    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;
    virtual ~Task() = default;

    void ignore() {
        ASSERT_SOFT(this->isBusy(), "task must be busy to be ignored");
        if (this->isBusy()) {
            spdlog::debug("<{}> Ignored a task...", getTaskName());
            std::lock_guard<std::mutex> lock(globals::ignoredFutures->mutex);
            globals::ignoredFutures->futures.push_back(std::move(this->future));
        }
    }

protected:
    constexpr virtual std::string getTaskName() = 0;

    void spawnTask(TaskFunction<TResult> task) {
        std::string taskName = getTaskName();
        auto onSuccess = [taskName](const TResult&) {
            spdlog::debug("<{}> Task complete, sending refresh signal...", taskName);
            globals::engine->sendRefreshSignal();
        };
        auto onFailure = [taskName](std::string_view errorMsg) {
            spdlog::error("<{}> Error detected: {}", taskName, errorMsg);
        };
        this->spawnTaskWithCallbacks(std::move(task), std::move(onSuccess), std::move(onFailure));
    }
};
