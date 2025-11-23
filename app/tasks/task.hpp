#pragma once

#include <spdlog/spdlog.h>

#include "globals/engine_state.hpp"
#include "globals/ignored_futures.hpp"
#include "utils/assertions.hpp"
#include "utils/async_worker.hpp"

template <typename TResult>
class Task : public virtual AsyncWorker<TResult> {
public:
    Task() = default;
    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;

    virtual ~Task() { ignore(); }

    void ignoreCurrentTask() {
        ignore();
        spdlog::debug("<{}> This task has been ignored...", getTaskId());
    }

protected:
    [[nodiscard]] virtual std::string getTaskId() = 0;

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

private:
    void ignore() {
        if (this->isBusy()) {
            std::lock_guard<std::mutex> lock(globals::ignoredFutures->mutex);
            globals::ignoredFutures->futures.push_back(std::move(this->future));
        }
        ASSERT(this->isAvailable(), "task must be free after ignore");
    }
};
