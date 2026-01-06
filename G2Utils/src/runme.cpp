#include <cstdint>
#include <filesystem>

#include <imgui.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/std.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "G2/ConfigManager.hpp"
#include "G2/ConsoleManager.hpp"
#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"
#include "G2/PlayerManager.hpp"
#include "G2/StringManager.hpp"
#include "defer.hpp"

#include "UI/ItemList.hpp"
#include "UI/PlayerList.hpp"
#include "UI/TableList.hpp"
#include "UI/Window.hpp"

void init_loggers();
void deinit_loggers();

void runme() {
  ConsoleManager::init();
  defer(ConsoleManager::deinit());

  init_loggers();
  defer(deinit_loggers());

  spdlog::info("Hello from the dll pickle!");

  StringManager::init();
  defer(StringManager::deinit());

  ConfigManager::init();
  defer(ConfigManager::deinit());

  PlayerManager::init();
  defer(PlayerManager::deinit());

  DataTableManager::init();
  defer(DataTableManager::deinit());

  ItemManager::init();
  defer(ItemManager::deinit());

  auto &console = ConsoleManager::get();
  auto &config = ConfigManager::get().config();

  Window window;

  auto player_list = PlayerList();
  auto table_list = TableList();
  auto item_list = ItemList(table_list);

  while (!window.exit_requested()) {
    window.begin_frame();
    defer(window.end_frame());

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

        PlayerManager::get().give_item_to_player(
            *player_list.selected_player(), *item_list.selected_item(), count);
      }

      if (!can_spawn_item)
        ImGui::EndDisabled();
    }
    ImGui::End();
  }
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

void deinit_loggers() {
  spdlog::info("Exiting...");
  spdlog::default_logger()->flush();
  spdlog::shutdown();
}
