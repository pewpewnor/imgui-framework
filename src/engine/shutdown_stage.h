#pragma once

#include "rigging.h"
#include "stage.h"

namespace engine {

class ShutdownStage : public virtual engine::Stage {
public:
    virtual void onShutdown(
        const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
