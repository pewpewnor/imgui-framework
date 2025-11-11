#pragma once

namespace engine {

class Stage {
public:
    Stage() = default;

    Stage(const Stage&) = default;
    Stage(Stage&&) = delete;
    Stage& operator=(const Stage&) = default;
    Stage& operator=(Stage&&) = delete;
    virtual ~Stage() = default;
};

}
