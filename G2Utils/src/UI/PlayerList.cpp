#include "UI/PlayerList.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "G2/PlayerManager.hpp"
#include "defer.hpp"

auto PlayerList::refresh() -> void {
  PlayerManager::get().refresh();

  if (m_selected_player.has_value() &&
      !PlayerManager::get().is_valid_player(*m_selected_player)) {
    m_selected_player = std::nullopt;
  }
}

auto PlayerList::render() -> void {
  if (ImGui::BeginListBox("Player List")) {
    defer(ImGui::EndListBox());

    auto players = PlayerManager::get().get_all_players();

    for (auto &handle : players) {
      if (ImGui::Selectable(fmt::format("{}", handle).c_str(),
                            m_selected_player.has_value() &&
                                m_selected_player->id() == handle.id())) {
        spdlog::info("Selected player changed to {}", handle);
        m_selected_player = handle;
      }
    }
  }

  if (m_selected_player.has_value()) {
    ImGui::Text("Selected player: %s",
                fmt::to_string(*m_selected_player).c_str());
  } else {
    ImGui::Text("Selected player: None");
  }
}
