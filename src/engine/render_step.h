#pragma once

#include "step.h"

namespace engine {

class RenderStep : public virtual engine::Step {
public:
    virtual void onRender() = 0;
};

}
