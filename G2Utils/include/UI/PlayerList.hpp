#pragma once

#include <cstddef>
#include <vector>

#include "G2/PlayerManager.hpp"

class PlayerList final {
public:
  auto refresh() -> void;
  auto render() -> void;

private:
  std::vector<PlayerHandle> m_players;
  size_t m_selected_player_idx;
};
