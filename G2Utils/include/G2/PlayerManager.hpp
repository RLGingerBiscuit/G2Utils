#pragma once

#include <optional>

#include "G2/ItemHandle.hpp"
#include "G2/PlayerHandle.hpp"
#include "G2/Singleton.hpp"

class PlayerManager final : public Singleton<PlayerManager> {
  friend class Singleton<PlayerManager>;

public:
  // Reloads all players from the game.
  auto refresh() -> void;

  // Check whether the given PlayerHandle refers to a valid player.
  //
  // NOTE: Unlike other validation functions, this checks whether the player
  // currently exists in the game, since the player list is expected to change.
  auto is_valid_player(const PlayerHandle &handle) -> bool;

  // Retrieves the first cached player that matches the given predicate.
  auto get_player_by(std::function<bool(const PlayerHandle &)> predicate)
      -> std::optional<PlayerHandle>;
  // Retrieves the cached player with the given ID.
  auto get_player_by_id(int id) -> std::optional<PlayerHandle>;
  // Retrieves the cached player with the given name.
  auto get_player_by_name(const std::string &name)
      -> std::optional<PlayerHandle>;

  // Retrieves all cached players.
  auto get_all_players() -> std::vector<PlayerHandle>;
  // Retrieves all cached players that match the given predicate.
  auto get_all_players_by(std::function<bool(const PlayerHandle &)> predicate)
      -> std::vector<PlayerHandle>;

  // Gives the specified item to the specified player.
  auto give_item_to_player(const PlayerHandle &player, const ItemHandle &item,
                           int count) -> bool;

private:
  std::vector<PlayerHandle> m_player_cache;
};
