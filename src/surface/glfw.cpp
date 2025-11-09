#include "glfw.h"

#include <GLFW/glfw3.h>

bool glfw::init() { return glfwInit() == 1; }

void glfw::setErrorCallback(GLFWerrorfun callback) {
    glfwSetErrorCallback(callback);
}

glfw::Window* glfw::createWindow(int width, int height,
                                 const std::string& title) {
    return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

glfw::Monitor* glfw::getPrimaryMonitor() { return glfwGetPrimaryMonitor(); }

void glfw::terminate() { glfwTerminate(); }
