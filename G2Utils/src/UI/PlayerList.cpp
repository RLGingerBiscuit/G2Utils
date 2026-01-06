#include "UI/PlayerList.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "G2/PlayerManager.hpp"
#include "defer.hpp"

auto PlayerList::refresh() -> void {
  auto new_players = PlayerManager::get().get_all_players();

  if (m_selected_player.has_value()) {
    bool found = std::any_of(
        new_players.begin(), new_players.end(),
        [this](auto &handle) { return handle == *m_selected_player; });

    if (!found)
      m_selected_player = {};
  }

  m_players = new_players;
}

auto PlayerList::render() -> void {
  if (ImGui::BeginListBox("Player List")) {
    defer(ImGui::EndListBox());

    for (auto &player : m_players) {
      if (ImGui::Selectable(fmt::format("{}", player).c_str(),
                            m_selected_player.has_value() &&
                                m_selected_player->id() == player.id())) {
        spdlog::info("Selected player changed to {}", player.name(),
                     player.id());
        m_selected_player = player;
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
