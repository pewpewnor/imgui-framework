#pragma once

#include "rigging.h"
#include "stage.h"

namespace engine {

class StartupStage : public virtual engine::Stage {
public:
    virtual void onStartup(const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
