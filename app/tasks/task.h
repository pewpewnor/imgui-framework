#pragma once

#include "globals/engine_state.h"
#include "globals/ignored_futures.h"
#include "spdlog/spdlog.h"
#include "utils/async_worker.h"

template <typename TResult>
class Task : public virtual AsyncWorker<TResult> {
public:
    Task() = default;
    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;
    virtual ~Task() = default;

    void ignore() const {
        if (this->isBusy()) {
            spdlog::debug("<{}> Ignoring this particular task...", getTaskId());
            std::lock_guard<std::mutex> lock(globals::ignoredFutures->mutex);
            globals::ignoredFutures->futures.push_back(std::move(this->future));
        } else {
            ASSERT(false, "task must be busy to be ignored");
        }
    }

protected:
    [[nodiscard]] virtual std::string getTaskId() const = 0;

    void spawnTask(const TaskFunction<TResult>& task) {
        std::string taskId = getTaskId();
        auto onSuccess = [taskId](const TResult&) {
            spdlog::debug("<{}> Task complete, sending refresh signal...", taskId);
            globals::engine->sendRefreshSignal();
        };
        auto onFailure = [taskId](std::string_view errorMsg) {
            spdlog::error("<{}> Error detected: {}", taskId, errorMsg);
        };
        this->spawnTaskWithCallbacks(task, std::move(onSuccess), std::move(onFailure));
    }
};
