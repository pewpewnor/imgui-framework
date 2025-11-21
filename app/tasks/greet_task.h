#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "spdlog/spdlog.h"
#include "task.h"

class GreetTask : public Task<std::string> {
public:
    void execute(const std::string& name, int frame) {
        taskId++;
        this->spawnTask([taskName = getTaskName(), name, frame]() -> std::string {
            spdlog::debug("<{}> Frame is {}, sleeping...", taskName, frame);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if (frame > 500) {
                throw std::runtime_error("simulated runtime error for greetings");
            }
            return "Hello, " + name + "! (Frame: " + std::to_string(frame) + ")";
        });
    }

protected:
    std::string getTaskName() override { return "GreetTask#" + std::to_string(taskId); }

private:
    static inline unsigned int taskId = 0;
};
