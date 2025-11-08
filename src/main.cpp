#include <iostream>
#include <stdexcept>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

namespace {
void glfwErrorCallback(int errorCode, const char* description) {
    std::cerr << "GLFW Error " << errorCode << ": " << description << std::endl;
}

// RAII wrapper for GLFW initialization
class GLFWContext {
public:
    GLFWContext() {
        glfwSetErrorCallback(glfwErrorCallback);
        if (glfwInit() == 0) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    ~GLFWContext() { glfwTerminate(); }

    // Prevent copying and moving
    GLFWContext(const GLFWContext&) = delete;
    GLFWContext& operator=(const GLFWContext&) = delete;
    GLFWContext(GLFWContext&&) = delete;
    GLFWContext& operator=(GLFWContext&&) = delete;
};

// RAII wrapper for GLFW window
class GLFWWindowPtr {
public:
    explicit GLFWWindowPtr(GLFWwindow* window) : window_(window) {
        if (window_ == nullptr) {
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    ~GLFWWindowPtr() {
        if (window_ != nullptr) {
            glfwDestroyWindow(window_);
        }
    }

    [[nodiscard]] GLFWwindow* get() const { return window_; }

    // Prevent copying and moving
    GLFWWindowPtr(const GLFWWindowPtr&) = delete;
    GLFWWindowPtr& operator=(const GLFWWindowPtr&) = delete;
    GLFWWindowPtr(GLFWWindowPtr&&) = delete;
    GLFWWindowPtr& operator=(GLFWWindowPtr&&) = delete;

private:
    GLFWwindow* window_;
};

// RAII wrapper for ImGui context
class ImGuiContextWrapper {
public:
    ImGuiContextWrapper(GLFWwindow* window, const char* glslVersion) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
            throw std::runtime_error("Failed to initialize ImGui GLFW backend");
        }

        if (!ImGui_ImplOpenGL3_Init(glslVersion)) {
            ImGui_ImplGlfw_Shutdown();
            throw std::runtime_error(
                "Failed to initialize ImGui OpenGL3 backend");
        }
    }

    ~ImGuiContextWrapper() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // Prevent copying and moving
    ImGuiContextWrapper(const ImGuiContextWrapper&) = delete;
    ImGuiContextWrapper& operator=(const ImGuiContextWrapper&) = delete;
    ImGuiContextWrapper(ImGuiContextWrapper&&) = delete;
    ImGuiContextWrapper& operator=(ImGuiContextWrapper&&) = delete;
};

const char* getGLSLVersion() {
#if defined(IMGUI_IMPL_OPENGL_ES2)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    return "#version 100";
#elif defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return "#version 150";
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    return "#version 130";
#endif
}

}  // anonymous namespace

int main() {
    try {
        // Initialize GLFW with RAII
        GLFWContext glfwContext;

        // Setup OpenGL version
        const char* glslVersion = getGLSLVersion();

        // Create window with DPI scaling
        const float scale =
            ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
        constexpr int baseWidth = 1280;
        constexpr int baseHeight = 800;

        GLFWWindowPtr window(
            glfwCreateWindow(static_cast<int>(baseWidth * scale),
                             static_cast<int>(baseHeight * scale),
                             "ImGui Application", nullptr, nullptr));

        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1);  // Enable vsync

        // Setup Dear ImGui with RAII
        ImGuiContextWrapper imguiContext(window.get(), glslVersion);

        ImGuiIO& imguiIO = ImGui::GetIO();
        imguiIO.ConfigFlags = static_cast<int>(
            static_cast<unsigned int>(imguiIO.ConfigFlags) |
            static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(scale);

        // Application state
        bool showDemoWindow = true;
        ImVec4 clearColor(0.45F, 0.55F, 0.60F, 1.0F);

        // Main loop
        while (glfwWindowShouldClose(window.get()) == 0) {
            glfwPollEvents();

            // Skip rendering if minimized
            if (glfwGetWindowAttrib(window.get(), GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Demo window
            if (showDemoWindow) {
                ImGui::ShowDemoWindow(&showDemoWindow);
            }

            // Custom window
            {
                static float sliderValue = 0.0F;
                static int counter = 0;

                ImGui::Begin("Hello, world!");
                ImGui::TextUnformatted("This is some useful text.");
                ImGui::Checkbox("Demo Window", &showDemoWindow);
                ImGui::SliderFloat("float", &sliderValue, 0.0F, 1.0F);
                ImGui::ColorEdit3("clear color", &clearColor.x);

                if (ImGui::Button("Button")) {
                    counter++;
                }
                ImGui::SameLine();

                const std::string counterText =
                    "counter = " + std::to_string(counter);
                ImGui::TextUnformatted(counterText.c_str());

                const std::string fpsText =
                    "Application average " +
                    std::to_string(1000.0F / imguiIO.Framerate) +
                    " ms/frame (" + std::to_string(imguiIO.Framerate) + " FPS)";
                ImGui::TextUnformatted(fpsText.c_str());
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            int displayW = 0;
            int displayH = 0;
            glfwGetFramebufferSize(window.get(), &displayW, &displayH);
            glViewport(0, 0, displayW, displayH);
            glClearColor(clearColor.x * clearColor.w,
                         clearColor.y * clearColor.w,
                         clearColor.z * clearColor.w, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window.get());
        }

        // Cleanup handled automatically by RAII wrappers
        return 0;

    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}
