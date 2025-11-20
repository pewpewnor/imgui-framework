#pragma once

#include "sleep_task.h"

struct Tasks {
    SleepTask sleepTask;
};

namespace globals {

inline std::unique_ptr<Tasks> tasks;

}
