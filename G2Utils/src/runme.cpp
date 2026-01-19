#include <cstdint>
#include <filesystem>

#include <imgui.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/std.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Dump/DataTableDumper.hpp"
#include "G2/ConfigManager.hpp"
#include "G2/ConsoleManager.hpp"
#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"
#include "G2/PlayerManager.hpp"
#include "G2/StringManager.hpp"
#include "G2/ThemeManager.hpp"
#include "defer.hpp"

#include "UI/ItemList.hpp"
#include "UI/PlayerList.hpp"
#include "UI/TableList.hpp"
#include "UI/Window.hpp"

auto init_loggers() -> void;
auto deinit_loggers() -> void;

auto runme() -> void {
  defer(deinit_loggers());

  ConsoleManager::init();
  defer(ConsoleManager::deinit());

  ConfigManager::init();
  defer(ConfigManager::deinit());

  init_loggers();

  spdlog::info("Hello from the dll pickle!");

  StringManager::init();
  defer(StringManager::deinit());

  PlayerManager::init();
  defer(PlayerManager::deinit());

  DataTableManager::init();
  defer(DataTableManager::deinit());

  ItemManager::init();
  defer(ItemManager::deinit());

  ThemeManager::init();
  defer(ThemeManager::deinit());

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

      if (ImGui::Button("Refresh")) {
        player_list.refresh(); // TODO: refresh players as they join/leave
        table_list.refresh();
        item_list.refresh();
      }
    }
    ImGui::End();

    if (ImGui::Begin("Debug")) {
      const auto &selected_table = table_list.selected_table();
      if (!selected_table)
        ImGui::BeginDisabled();
      if (ImGui::Button("Dump")) {
        auto dumper = DataTableDumper(*selected_table);
        dumper.dump();
      }
      if (!selected_table)
        ImGui::EndDisabled();

      if (ImGui::Button("Dump All Tables")) {
        DataTableDumper::dump_all();
      }

#if !NDEBUG
      if (ImGui::Button("Force Crash")) {
        *((volatile uint64_t *)0) = 34 + 35;
      }
    }
#endif
    ImGui::End();

    if (ImGui::Begin("Item Spawner")) {
      auto can_spawn_item = player_list.selected_player().has_value() &&
                            table_list.selected_table().has_value() &&
                            item_list.selected_item().has_value();

      static int count = 1;

      ImGui::InputInt("Count", &count);

      player_list.render();
      ImGui::Spacing();
      table_list.render();
      ImGui::Spacing();
      item_list.render();
      ImGui::Spacing();

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

auto init_loggers() -> void {
  const auto console_sink =
      std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(spdlog::level::info);

  const auto log_path = ConfigManager::get().log_path() / "log.txt";

  const auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      log_path.string(), 5 * (1 << 20), 5);
  file_sink->set_level(spdlog::level::trace);

  const auto logger = std::make_shared<spdlog::logger>(
      "G2Utils", spdlog::sinks_init_list{console_sink, file_sink});
  spdlog::set_default_logger(logger);

  spdlog::info("Log path: '{}'", log_path);
}

auto deinit_loggers() -> void {
  spdlog::info("Exiting...");
  spdlog::default_logger()->flush();
  spdlog::shutdown();
}
