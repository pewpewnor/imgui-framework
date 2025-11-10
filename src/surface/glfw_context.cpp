#include "glfw_context.h"

#include <iostream>

#include "glfw.h"

surface::GlfwContext::GlfwContext() {
    glfw::setErrorCallback([](int errorCode, const char* description) {
        std::cerr << "GLFW Error " << errorCode << ": " << description
                  << std::endl;
    });
    if (!glfw::init()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

surface::GlfwContext::~GlfwContext() { glfw::terminate(); }
