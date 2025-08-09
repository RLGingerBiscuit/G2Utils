#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Maine_structs.hpp>

#include "G2/PlayerManager.hpp"

SINGLETON_IMPL(PlayerManager);

auto PlayerManager::init_singleton() -> void {}

auto PlayerManager::deinit_singleton() -> void {}

auto PlayerManager::get_player_by_id(int32_t target_id)
    -> std::optional<PlayerHandle> {
  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr)
    return {};

  auto *state = world->GameState;
  if (state == nullptr)
    return {};

  for (int i = 0; i < state->PlayerArray.Num(); ++i) {
    auto *player_state = state->PlayerArray[i];
    if (player_state == nullptr)
      continue;

    auto player_name = player_state->GetPlayerName().ToString();

    if (player_state->PlayerId == target_id)
      return PlayerHandle(player_state->PlayerId, player_name);
  }

  return {};
}

auto PlayerManager::get_player_by_name(std::string target_name)
    -> std::optional<PlayerHandle> {
  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr)
    return {};

  auto *state = world->GameState;
  if (state == nullptr)
    return {};

  for (int i = 0; i < state->PlayerArray.Num(); ++i) {
    auto *player_state = state->PlayerArray[i];
    if (player_state == nullptr)
      continue;

    auto player_name = player_state->GetPlayerName().ToString();

    if (player_name == target_name)
      return PlayerHandle(player_state->PlayerId, player_name);
  }

  return {};
}

auto PlayerManager::get_all_players() -> std::vector<PlayerHandle> {
  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr)
    return {};

  auto *state = world->GameState;
  if (state == nullptr)
    return {};

  std::vector<PlayerHandle> players;
  players.reserve(state->PlayerArray.Num());

  for (int i = 0; i < state->PlayerArray.Num(); ++i) {
    auto *player_state = state->PlayerArray[i];
    if (player_state == nullptr)
      continue;

    auto player_name = player_state->GetPlayerName().ToString();

    players.push_back(PlayerHandle(player_state->PlayerId, player_name));
  }

  return players;
}
