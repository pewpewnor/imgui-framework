#include "greet_task.h"

void GreetTask::execute(const std::string& name, int frame) {
    this->spawnTask([taskId = getTaskId(), name, frame]() -> std::string {
        spdlog::debug("<{}> Frame is {}, sleeping...", taskId, frame);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (frame > 500) {
            throw std::runtime_error("simulated runtime error for greetings");
        }
        return "Hello, " + name + "! (Frame: " + std::to_string(frame) + ")";
    });
}

std::string GreetTask::getTaskId() const { return "GreetTask"; }
