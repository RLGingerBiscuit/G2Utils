#pragma once

#include <cstdint>
#include <optional>

#include "G2/ItemHandle.hpp"
#include "G2/PlayerHandle.hpp"
#include "G2/Singleton.hpp"

// FIXME: Preferably, I want to keep all mentions of the SDK functions to the
//        .cpp files. This ain't that.
namespace SDK {
class APlayerState;
}

class PlayerManager final : public Singleton<PlayerManager> {
  friend class Singleton<PlayerManager>;

public:
  auto get_player_by_id(int32_t target_id) -> std::optional<PlayerHandle>;
  auto get_player_by_name(std::string target_name)
      -> std::optional<PlayerHandle>;
  auto get_all_players() -> std::vector<PlayerHandle>;

  auto give_item_to_player(PlayerHandle player, ItemHandle item, int count)
      -> bool;

  auto get_player_state(PlayerHandle handle) -> SDK::APlayerState *;

  auto is_valid_player(PlayerHandle player) -> bool;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
