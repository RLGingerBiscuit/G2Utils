#include <imgui.h>

#include "G2/PlayerManager.hpp"
#include "defer.hpp"

#include "UI/PlayerList.hpp"

auto PlayerList::refresh() -> void {
  auto new_players = PlayerManager::instance().get_all_players();

  if (new_players.size() < m_players.size() &&
      m_selected_player_idx >= new_players.size()) {
    m_selected_player_idx = new_players.size() - 1;
  }

  m_players = new_players;
}

auto PlayerList::render() -> void {
  if (ImGui::BeginListBox("Player List")) {
    defer(ImGui::EndListBox());

    for (size_t i = 0; i < m_players.size(); ++i) {
      auto &player = m_players[i];
      if (ImGui::Selectable(
              fmt::format("'{}' (ID: {})", player.name(), player.id()).c_str()),
          m_selected_player_idx == i) {
        spdlog::info("Selected player changed to {} ({})", player.name(),
                     player.id());
        m_selected_player_idx = i;
      }
    }
  }

  if (m_players.size() > 0) {
    auto &selected_player = m_players[m_selected_player_idx];
    ImGui::Text("Selected player: '%s' (ID: %d)",
                selected_player.name().c_str(), selected_player.id());
  } else {
    ImGui::Text("Selected player: None");
  }
}
