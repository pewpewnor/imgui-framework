#pragma once

#include "tasks/greet_task.h"

namespace globals {

struct Tasks {
    GreetTask greetTask;
};

inline std::unique_ptr<Tasks> tasks;

}
