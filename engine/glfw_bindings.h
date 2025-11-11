#pragma once

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include <memory>

namespace glfw {

using Window = GLFWwindow;
using Monitor = GLFWmonitor;

bool init();

std::shared_ptr<glfw::Window> createWindow(int width, int height,
                                           const std::string& title);

bool windowShouldClose(const std::shared_ptr<glfw::Window>& window);

bool windowAttributeIsError(const std::shared_ptr<glfw::Window>& window);

void setWindowHint(int hint, int value);

std::shared_ptr<glfw::Monitor> getPrimaryMonitor();

void makeContextCurrent(const std::shared_ptr<glfw::Window>& window);

void swapBuffers(const std::shared_ptr<glfw::Window>& window);

void switchVsync(bool vsyncOn);

void pollEvents();

void setErrorCallback(GLFWerrorfun callback);

std::string glslVersion();

void terminate();

}
