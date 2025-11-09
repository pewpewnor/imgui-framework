#include "application.h"

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>

Application::Application()
    : glfw_context_(nullptr),
      window_(nullptr),
      imgui_context_(nullptr),
      show_demo_window_(true),
      is_initialized_(false),
      clear_color_(0.45F, 0.55F, 0.60F, 1.0F),
      counter_(0),
      slider_value_(0.0F),
      scale_(1.0F) {}

Application::~Application() { shutdown(); }

bool Application::initialize() {
    try {
        glfw_context_ = std::make_unique<surface::GlfwContext>();

        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        if (primary != nullptr) {
            scale_ = ImGui_ImplGlfw_GetContentScaleForMonitor(primary);
        }

        window_ = std::make_unique<surface::GlfwWindow>(
            static_cast<int>(1280 * scale_), static_cast<int>(800 * scale_),
            "Example App");

        imgui_context_ = std::make_unique<surface::ImGuiContext>(*window_);

        if (!initializeImGui()) {
            return false;
        }

        is_initialized_ = true;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Initialization error: " << e.what() << std::endl;
        return false;
    }
}

bool Application::initializeImGui() {
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags = static_cast<int>(
        static_cast<unsigned int>(io.ConfigFlags) |
        static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(scale_);

    return true;
}

void Application::run() {
    if (!is_initialized_) {
        std::cerr << "Application not initialized. Call initialize() first."
                  << std::endl;
        return;
    }

    while (glfwWindowShouldClose(window_->get()) == 0) {
        handleInput();

        if (glfwGetWindowAttrib(window_->get(), GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        renderFrame();
    }
}

void Application::handleInput() { glfwPollEvents(); }

void Application::renderFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderUI();

    ImGui::Render();

    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(window_->get(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color_.x * clear_color_.w,
                 clear_color_.y * clear_color_.w,
                 clear_color_.z * clear_color_.w, clear_color_.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window_->get());
}

void Application::renderUI() {
    ImGuiIO& io = ImGui::GetIO();

    if (show_demo_window_) {
        ImGui::ShowDemoWindow(&show_demo_window_);
    }

    ImGui::Begin("Hello, world!");

    ImGui::TextUnformatted("This is some useful text.");
    ImGui::Checkbox("Demo Window", &show_demo_window_);
    ImGui::SliderFloat("float", &slider_value_, 0.0F, 1.0F);
    ImGui::ColorEdit3("clear color", &clear_color_.x);

    if (ImGui::Button("Button")) {
        counter_++;
    }
    ImGui::SameLine();

    const std::string counter_text = "counter = " + std::to_string(counter_);
    ImGui::TextUnformatted(counter_text.c_str());

    const std::string fps_text =
        "Application average " + std::to_string(1000.0F / io.Framerate) +
        " ms/frame (" + std::to_string(io.Framerate) + " FPS)";
    ImGui::TextUnformatted(fps_text.c_str());

    ImGui::End();
}

void Application::shutdown() {
    imgui_context_.reset();
    window_.reset();
    glfw_context_.reset();
    is_initialized_ = false;
}
