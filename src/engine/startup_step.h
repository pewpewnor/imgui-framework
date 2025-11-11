#pragma once

#include "rigging.h"
#include "step.h"

namespace engine {

class StartupStep : public virtual engine::Step {
public:
    virtual void onStartup(const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
