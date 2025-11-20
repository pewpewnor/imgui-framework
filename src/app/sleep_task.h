#pragma once

#include "spdlog/spdlog.h"
#include "states.h"
#include "utils/async_task.h"

class SleepTask : public AsyncTask<std::string> {
public:
    void run(const std::string& name, int frameCount) {
        inputName_ = name;
        inputFrame_ = frameCount;
        this->spawn();
    }

protected:
    void execute() override {
        spdlog::debug("Worker: Starting sleep...");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::string res = "Hello, " + inputName_ + " (Frame: " + std::to_string(inputFrame_) + ")";
        this->submitResult(std::move(res));
    }

    void postTask() override {
        spdlog::debug("Worker: Sending refresh signal...");
        globals::engine->sendRefreshSignal();
    }

private:
    std::string inputName_;
    int inputFrame_ = 0;
};
