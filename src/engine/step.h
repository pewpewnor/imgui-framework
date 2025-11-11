#pragma once

namespace engine {

class Step {
public:
    Step() = default;

    Step(const Step&) = default;
    Step(Step&&) = delete;
    Step& operator=(const Step&) = default;
    Step& operator=(Step&&) = delete;
    virtual ~Step() = default;
};

}
