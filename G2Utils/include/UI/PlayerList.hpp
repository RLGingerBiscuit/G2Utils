#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "G2/PlayerManager.hpp"

class PlayerList final {
public:
  auto refresh() -> void;
  auto render() -> void;

  auto selected_player() -> std::optional<PlayerHandle> {
    if (m_selected_player_idx >= m_players.size())
      return {};
    return m_players[m_selected_player_idx];
  }

private:
  std::vector<PlayerHandle> m_players;
  size_t m_selected_player_idx;
};
