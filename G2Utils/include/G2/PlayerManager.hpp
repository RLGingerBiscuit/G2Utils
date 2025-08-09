#pragma once

#include <cstdint>
#include <optional>

#include "G2/PlayerHandle.hpp"
#include "G2/Singleton.hpp"

class PlayerManager final : public Singleton<PlayerManager> {
  friend class Singleton<PlayerManager>;

public:
  auto get_player_by_id(int32_t target_id) -> std::optional<PlayerHandle>;
  auto get_player_by_name(std::string target_name)
      -> std::optional<PlayerHandle>;
  auto get_all_players() -> std::vector<PlayerHandle>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
