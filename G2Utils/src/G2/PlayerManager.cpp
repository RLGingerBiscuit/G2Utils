#include <SDK/SDK/BP_SurvivalGameMode_classes.hpp>
#include <SDK/SDK/BP_SurvivalPlayerCharacter_classes.hpp>
#include <SDK/SDK/Basic.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Engine_structs.hpp>
#include <SDK/SDK/Maine_structs.hpp>

#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"
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

auto PlayerManager::give_item_to_player(PlayerHandle player, ItemHandle item,
                                        int count) -> bool {
  if (!is_valid_player(player))
    return false;
  if (!ItemManager::get().is_valid_item(item))
    return false;

  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr)
    return false;

  auto *game_mode_base = world->AuthorityGameMode;
  if (game_mode_base == nullptr ||
      !game_mode_base->IsA(SDK::ABP_SurvivalGameMode_C::StaticClass()))
    return false;

  auto *game_mode = static_cast<SDK::ABP_SurvivalGameMode_C *>(game_mode_base);

  auto players = get_all_players();

  for (auto &player : players) {
    auto player_state = get_player_state(player);
    if (player_state == nullptr || player_state->PlayerId != player.id())
      continue;

    auto *pawn = player_state->PawnPrivate;
    if (pawn == nullptr ||
        !pawn->IsA(SDK::ABP_SurvivalPlayerCharacter_C::StaticClass()))
      continue;

    auto *character = static_cast<SDK::ABP_SurvivalPlayerCharacter_C *>(pawn);

    SDK::FDataTableRowHandle row_handle = {};

    auto table_and_row = DataTableManager::get().get_table_and_row(item);

    // TODO: reduce searching (wrappers maybe?)
    row_handle.DataTable = table_and_row.first;
    row_handle.RowName = table_and_row.second;

    game_mode->GrantItemsToPlayer(character, row_handle, count);

    return true;
  }

  return false;
}

auto PlayerManager::get_player_state(PlayerHandle handle)
    -> SDK::APlayerState * {
  if (!PlayerManager::is_valid_player(handle))
    return nullptr;

  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr)
    return nullptr;

  auto *state = world->GameState;
  if (state == nullptr)
    return nullptr;

  for (int i = 0; i < state->PlayerArray.Num(); ++i) {
    auto *player_state = state->PlayerArray[i];
    if (player_state == nullptr || player_state->PlayerId != handle.id())
      continue;

    return player_state;
  }

  return nullptr;
}

auto PlayerManager::is_valid_player(PlayerHandle player) -> bool {
  auto handle = get_player_by_id(player.id());
  return handle.has_value();
}
