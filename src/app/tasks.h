#pragma once

#include "greet_task.h"

struct Tasks {
    GreetTask greetTask;
};

namespace globals {

inline std::unique_ptr<Tasks> tasks;

}
