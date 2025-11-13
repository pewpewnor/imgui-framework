#pragma once

namespace engine {

class Step {
public:
    Step() = default;
    Step(const Step&) = default;
    Step(Step&&) = default;
    Step& operator=(const Step&) = default;
    Step& operator=(Step&&) = default;
    virtual ~Step() = default;
};

}
