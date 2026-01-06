#pragma once

#include <optional>
#include <vector>

#include "G2/PlayerHandle.hpp"

class PlayerList final {
public:
  auto refresh() -> void;
  auto render() -> void;

  auto selected_player() -> std::optional<PlayerHandle> {
    return m_selected_player;
  }

private:
  std::vector<PlayerHandle> m_players;
  std::optional<PlayerHandle> m_selected_player;
};
