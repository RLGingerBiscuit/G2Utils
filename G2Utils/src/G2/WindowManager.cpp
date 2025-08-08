#include <glad/glad.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "G2/WindowManager.hpp"
#include "GLFW/glfw3.h"

SINGLETON_IMPL(WindowManager);

static void glfw_error_callback(int error, const char *desc) {}

auto WindowManager::init_singleton() -> void {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    spdlog::error("Could not initialise glfw");
    return;
  }

  spdlog::info("Initialised GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window_title = fmt::format("G2Utils (OpenGL3)");
  m_window =
      glfwCreateWindow(1280, 800, window_title.c_str(), nullptr, nullptr);
  if (m_window == nullptr) {
    spdlog::error("Could not create window");
    return;
  }
  spdlog::info("Created GLFW window: {:p}", (void *)m_window);

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  if (!gladLoadGL(glfwGetProcAddress)) {
    spdlog::error("Could not initialize glad");
    return;
  }
  spdlog::info("Loaded Glad");

  static constexpr auto glsl_version = "#version 150";

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  auto &style = ImGui::GetStyle();

  auto scale = get_window_scale();
  style.ScaleAllSizes(scale);
  style.FontScaleDpi = scale;

  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  spdlog::info("Initialised ImGui");
}

auto WindowManager::deinit_singleton() -> void {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

auto WindowManager::begin_frame() -> void {
  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

auto WindowManager::end_frame() -> void {
  ImGui::Render();

  int width = 0, height = 0;
  glfwGetFramebufferSize(m_window, &width, &height);
  glViewport(0, 0, width, height);
  glClearColor(0x11 / (float)0xff, 0x11 / (float)0xff, 0x11 / (float)0xff, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(m_window);
}

auto WindowManager::exit_requested() -> bool {
  return glfwWindowShouldClose(m_window);
}

auto WindowManager::request_exit() -> void {
  glfwSetWindowShouldClose(m_window, true);
}

auto WindowManager::get_window_scale() -> float {
  return ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
}
