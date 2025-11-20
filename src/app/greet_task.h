#pragma once

#include "spdlog/spdlog.h"
#include "states.h"
#include "utils/async_task.h"

class GreetTask : public AsyncTask<std::string> {
public:
    void run(const std::string& name, int frame) {
        name_ = name;
        frame_ = frame;
        this->spawn();
    }

private:
    std::string name_;
    int frame_;

    void task() override {
        spdlog::debug("Sleep worker: starting sleep...");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::string res = "Hello, " + name_ + "! (Frame: " + std::to_string(frame_) + ")";
        this->submitResult(res);
    }

    void afterTask() override {
        spdlog::debug("Sleep worker: sending refresh signal...");
        globals::engine->sendRefreshSignal();
    }
};
