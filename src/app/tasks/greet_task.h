#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "spdlog/spdlog.h"
#include "task.h"

class GreetTask : public Task<std::string> {
public:
    GreetTask() : Task("GreetTask") {}

    void execute(const std::string& name, int frame) {
        this->runTask([name, frame]() -> std::string {
            spdlog::debug("<Greet Task> Starting sleep...");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            if (frame > 300) {
                throw std::runtime_error("simulated runtime error for greetings");
            }
            std::string res = "Hello, " + name + "! (Frame: " + std::to_string(frame) + ")";
            return res;
        });
    }
};
