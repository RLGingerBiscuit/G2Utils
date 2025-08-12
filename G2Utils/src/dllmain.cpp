#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <filesystem>

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/std.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <imgui.h>

#include "G2/ConfigManager.hpp"
#include "G2/ConsoleManager.hpp"
#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"
#include "G2/PlayerManager.hpp"
#include "G2/WindowManager.hpp"
#include "defer.hpp"

#include "UI/ItemList.hpp"
#include "UI/PlayerList.hpp"
#include "UI/TableList.hpp"

#define UNUSED(x) ((void)x);

static HANDLE g_thread = nullptr;
static bool g_running = false;

void init_loggers();

void run() {
  ConsoleManager::init();
  defer(ConsoleManager::deinit());

  init_loggers();

  spdlog::info("Hello from the dll pickle!");

  ConfigManager::init();
  defer(ConfigManager::deinit());

  WindowManager::init();
  defer(WindowManager::deinit());

  PlayerManager::init();
  defer(PlayerManager::deinit());

  DataTableManager::init();
  defer(DataTableManager::deinit());

  ItemManager::init();
  defer(ItemManager::deinit());

  auto &console = ConsoleManager::instance();
  auto &window = WindowManager::instance();
  auto &config = ConfigManager::instance().config();

  auto player_list = PlayerList();
  auto table_list = TableList();
  auto item_list = ItemList(table_list);

  while (!window.exit_requested()) {
    window.begin_frame();

    if (ImGui::Begin("Utils")) {
      if (ImGui::Checkbox("Console", &config.show_console)) {
        if (config.show_console)
          console.show();
        else
          console.hide();
      }

      // NOTE: This is here because the game doesn't quite close properly
      // sometimes (EA amirite?).
      //       It's simply quicker to crash the game than force close it.
      if (ImGui::Button("Force Crash")) {
        *((volatile uint64_t *)0) = 34 + 35;
      }
    }
    ImGui::End();

    if (ImGui::Begin("Item Spawner")) {
      if (ImGui::Button("Refresh")) {
        player_list.refresh();
        // FIXME: We really should just not crash on refresh in the first place
        if (!table_list.has_inited_tables())
          table_list.refresh();
        item_list.refresh();
      }

      auto can_spawn_item = player_list.selected_player().has_value() &&
                            table_list.selected_table().has_value() &&
                            item_list.selected_item().has_value();

      static int count = 1;

      ImGui::SameLine();

      ImGui::InputInt("Count", &count);

      player_list.render();
      table_list.render();
      item_list.render();

      if (!can_spawn_item)
        ImGui::BeginDisabled();

      if (ImGui::Button("Spawn")) {
        assert(player_list.selected_player().has_value());
        assert(table_list.selected_table().has_value());
        assert(item_list.selected_item().has_value());
        PlayerManager::instance().give_item_to_player(
            *player_list.selected_player(), *item_list.selected_item(), count);
      }

      if (!can_spawn_item)
        ImGui::EndDisabled();
    }
    ImGui::End();

    window.end_frame();
  }
}

DWORD WINAPI ThreadEntry(LPVOID lpParam) {
  HMODULE local_module = 0;
  GetModuleHandleEx(0, nullptr, &local_module);

  SetThreadDescription(g_thread, L"G2Utils Thread");

  run();

  spdlog::info("Exiting...");
  spdlog::default_logger()->flush();
  spdlog::shutdown();

  g_thread = nullptr;
  g_running = false;

  FreeLibraryAndExitThread(local_module, EXIT_SUCCESS);

  return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  switch (dwReason) {
  case DLL_PROCESS_ATTACH: {
    g_running = true;
    g_thread =
        CreateThread(nullptr, 0, ThreadEntry, (LPVOID)hModule, 0, nullptr);
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

  const auto log_path = std::filesystem::canonical(file_sink->filename());

  spdlog::info("Log path: '{}'", file_sink->filename());
}
