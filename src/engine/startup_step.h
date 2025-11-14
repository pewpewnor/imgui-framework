#pragma once

#include "step.h"

namespace engine {

class StartupStep : public virtual engine::Step {
public:
    virtual void onStartup() = 0;
};

}
