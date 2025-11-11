#include "glfw_bindings.h"

bool glfw::init() { return glfwInit() == GLFW_TRUE; }

void glfw::setErrorCallback(GLFWerrorfun callback) {
    glfwSetErrorCallback(callback);
}

std::shared_ptr<glfw::Window> glfw::createWindow(int width, int height,
                                                 const std::string& title) {
    glfw::Window* window =
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("failed to create GLFW window");
    }
    return std::shared_ptr<glfw::Window>(
        window, [](glfw::Window* win) { glfwDestroyWindow(win); });
}

bool glfw::windowShouldClose(const std::shared_ptr<glfw::Window>& window) {
    return glfwWindowShouldClose(window.get()) == 1;
}

bool glfw::windowAttributeIsError(const std::shared_ptr<glfw::Window>& window) {
    return glfwGetWindowAttrib(window.get(), GLFW_ICONIFIED) != 0;
}

void glfw::setWindowHint(int hint, int value) { glfwWindowHint(hint, value); }

std::shared_ptr<glfw::Monitor> glfw::getPrimaryMonitor() {
    glfw::Monitor* monitor = glfwGetPrimaryMonitor();
    if (monitor == nullptr) {
        throw std::runtime_error("failed to get GLFW primary monitor");
    }
    return std::shared_ptr<glfw::Monitor>(monitor, [](glfw::Monitor*) {});
}

void glfw::makeContextCurrent(const std::shared_ptr<glfw::Window>& window) {
    glfwMakeContextCurrent(window.get());
}

void glfw::swapBuffers(const std::shared_ptr<glfw::Window>& window) {
    glfwSwapBuffers(window.get());
}

void glfw::switchVsync(bool vsyncOn) { glfwSwapInterval(vsyncOn ? 1 : 0); }

void glfw::pollEvents() { glfwPollEvents(); }

std::string glfw::glslVersion() {
#if defined(IMGUI_IMPL_OPENGL_ES2)
    glfw::setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfw::setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfw::setWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    return "#version 100";
#elif defined(__APPLE__)
    glfw::setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfw::setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfw::setWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfw::setWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return "#version 150";
#else
    glfw::setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfw::setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfw::setWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfw::setWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return "#version 150";
#endif
}

void glfw::terminate() { glfwTerminate(); }
