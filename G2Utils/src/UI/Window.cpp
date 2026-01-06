#include "UI/Window.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

static void glfw_error_callback(int error, const char *desc) {
  std::string error_str;
  switch (error) {
  case GLFW_NO_ERROR:
    error_str = "NO_ERROR";
    break;
  case GLFW_NOT_INITIALIZED:
    error_str = "NOT_INITIALIZED";
    break;
  case GLFW_NO_CURRENT_CONTEXT:
    error_str = "NO_CURRENT_CONTEXT";
    break;
  case GLFW_INVALID_ENUM:
    error_str = "INVALID_ENUM";
    break;
  case GLFW_INVALID_VALUE:
    error_str = "INVALID_VALUE";
    break;
  case GLFW_OUT_OF_MEMORY:
    error_str = "OUT_OF_MEMORY";
    break;
  case GLFW_API_UNAVAILABLE:
    error_str = "API_UNAVAILABLE";
    break;
  case GLFW_VERSION_UNAVAILABLE:
    error_str = "VERSION_UNAVAILABLE";
    break;
  case GLFW_PLATFORM_ERROR:
    error_str = "PLATFORM_ERROR";
    break;
  case GLFW_FORMAT_UNAVAILABLE:
    error_str = "FORMAT_UNAVAILABLE";
    break;
  case GLFW_NO_WINDOW_CONTEXT:
    error_str = "NO_WINDOW_CONTEXT";
    break;
  case GLFW_CURSOR_UNAVAILABLE:
    error_str = "CURSOR_UNAVAILABLE";
    break;
  case GLFW_FEATURE_UNAVAILABLE:
    error_str = "FEATURE_UNAVAILABLE";
    break;
  case GLFW_FEATURE_UNIMPLEMENTED:
    error_str = "FEATURE_UNIMPLEMENTED";
    break;
  case GLFW_PLATFORM_UNAVAILABLE:
    error_str = "PLATFORM_UNAVAILABLE";
    break;
  default:
    error_str = "UNKNOWN";
    break;
  }

  spdlog::error("GLFW Error ({}): {}", error, desc);
}

Window::Window() {
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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();
  auto &io = ImGui::GetIO();
  auto &style = ImGui::GetStyle();

  // Enable docking
  io.ConfigFlags |=
      ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
  style.WindowRounding = 0;
  style.Colors[ImGuiCol_WindowBg].w = 1;

  // Enable scaling
  // io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts |
  //                   ImGuiConfigFlags_DpiEnableScaleViewports;

  auto scale = get_window_scale();
  style.ScaleAllSizes(scale);
  style.FontScaleDpi = scale;
  io.ConfigDpiScaleFonts = true;
  io.ConfigDpiScaleViewports = true;

  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  spdlog::info("Initialised ImGui");
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

auto Window::begin_frame() -> void {
  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    ImGui::DockSpaceOverViewport();
}

auto Window::end_frame() -> void {
  ImGui::Render();

  int width = 0, height = 0;
  glfwGetFramebufferSize(m_window, &width, &height);
  glViewport(0, 0, width, height);
  glClearColor(0x11 / (float)0xff, 0x11 / (float)0xff, 0x11 / (float)0xff, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    auto *backup = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup);
  }

  glfwSwapBuffers(m_window);
}

auto Window::exit_requested() -> bool {
  return glfwWindowShouldClose(m_window);
}

auto Window::request_exit() -> void {
  glfwSetWindowShouldClose(m_window, true);
}

auto Window::get_window_scale() -> float {
  return ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
}
