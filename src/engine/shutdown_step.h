#pragma once

#include "step.h"

namespace engine {

class ShutdownStep : public virtual engine::Step {
public:
    virtual void onShutdown() = 0;
};

}
