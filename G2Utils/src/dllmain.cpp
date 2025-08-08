#include "spdlog/common.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <filesystem>

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/std.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <imgui.h>

#include "G2/ConsoleManager.hpp"
#include "G2/WindowManager.hpp"

#define UNUSED(x) ((void)x);

static HANDLE g_thread = nullptr;
static bool g_running = false;

void init_loggers();

DWORD WINAPI ThreadEntry(LPVOID lpParam) {
  ConsoleManager::init();

  init_loggers();

  spdlog::info("Hello from the dll pickle!");

  auto &window = WindowManager::instance();

  while (!window.exit_requested()) {
    spdlog::info("Exit: {}", window.exit_requested());

    window.begin_frame();

    ImGui::ShowDemoWindow();

    window.end_frame();
  }

  spdlog::info("Exiting...");

  return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  switch (dwReason) {
  case DLL_PROCESS_ATTACH: {
    g_running = true;
    g_thread =
        CreateThread(nullptr, 0, ThreadEntry, (LPVOID)hModule, 0, nullptr);
  }; break;
  case DLL_PROCESS_DETACH: {
    g_running = false;
  }; break;
  default:
    break;
  }

  return TRUE;
}

void init_loggers() {
  const auto console_sink =
      std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(spdlog::level::info);

  const auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      "g2utils/logs/log.txt", 5 * (1 << 20), 5);
  file_sink->set_level(spdlog::level::trace);

  const auto logger = std::make_shared<spdlog::logger>(
      "", spdlog::sinks_init_list{console_sink, file_sink});
  spdlog::set_default_logger(logger);

  const auto log_filename = file_sink->calc_filename("logs/log.txt", 0);
  const auto log_path = std::filesystem::canonical(log_filename);

  spdlog::info("Log path: '{}'", log_path.parent_path());
}
