#pragma once

namespace surface {

class GlfwContext {
public:
    GlfwContext();

    ~GlfwContext();

    GlfwContext(const GlfwContext&) = delete;
    GlfwContext& operator=(const GlfwContext&) = delete;
    GlfwContext(GlfwContext&&) = delete;
    GlfwContext& operator=(GlfwContext&&) = delete;
};

}
