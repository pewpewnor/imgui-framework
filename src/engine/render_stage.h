#pragma once

#include "rigging.h"
#include "stage.h"

namespace engine {

class RenderLayer : public virtual engine::Stage {
public:
    virtual void onRender(const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
