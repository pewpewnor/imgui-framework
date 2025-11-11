#pragma once

#include "rigging.h"
#include "step.h"

namespace engine {

class ShutdownStep : public virtual engine::Step {
public:
    virtual void onShutdown(
        const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
