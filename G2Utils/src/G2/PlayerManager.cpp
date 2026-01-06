#include "G2/PlayerManager.hpp"

#include <SDK/SDK/BP_SurvivalGameMode_classes.hpp>
#include <SDK/SDK/BP_SurvivalPlayerCharacter_classes.hpp>
#include <SDK/SDK/Basic.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Engine_structs.hpp>
#include <SDK/SDK/Maine_structs.hpp>

#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"
#include "G2/StringManager.hpp"

SINGLETON_IMPL(PlayerManager);

static auto get_player_state(PlayerHandle handle) -> SDK::APlayerState * {
  if (!PlayerManager::get().is_valid_player(handle)) {
    spdlog::warn("Player {} was not valid", handle);
    return nullptr;
  }

  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr) {
    spdlog::warn("World was null");
    return nullptr;
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return nullptr;
  }

  for (auto *player_state : state->PlayerArray) {
    if (player_state == nullptr || player_state->PlayerId != handle.id())
      continue;
    return player_state;
  }

  spdlog::warn("Could not find state for player {}", handle);
  return nullptr;
}

auto PlayerManager::init_singleton() -> void {}

auto PlayerManager::deinit_singleton() -> void {}

auto PlayerManager::get_player_by_id(int32_t target_id)
    -> std::optional<PlayerHandle> {
  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr) {
    spdlog::warn("World was null");
    return {};
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return {};
  }

  for (auto *player_state : state->PlayerArray) {
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
  if (world == nullptr) {
    spdlog::warn("World was null");
    return {};
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return {};
  }

  for (auto *player_state : state->PlayerArray) {
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
  if (world == nullptr) {
    spdlog::warn("World was null");
    return {};
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return {};
  }

  std::vector<PlayerHandle> players;
  players.reserve(state->PlayerArray.Num());

  for (auto *player_state : state->PlayerArray) {
    if (player_state == nullptr)
      continue;
    auto player_name = player_state->GetPlayerName().ToString();
    players.push_back(PlayerHandle(player_state->PlayerId, player_name));
  }

  return players;
}

auto PlayerManager::give_item_to_player(PlayerHandle player, ItemHandle item,
                                        int count) -> bool {
  if (!is_valid_player(player)) {
    spdlog::error("Player {} is not valid", player);
    return false;
  }

  if (!ItemManager::get().is_valid_item(item)) {
    spdlog::error("Item {} is not valid", item);
    return false;
  }

  auto data_table = DataTableManager::get().get_table_raw(item.table_handle());
  assert(data_table.has_value());

  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr) {
    spdlog::warn("World was null");
    return false;
  }

  auto *game_mode_base = world->AuthorityGameMode;
  if (game_mode_base == nullptr ||
      !game_mode_base->IsA(SDK::ABP_SurvivalGameMode_C::StaticClass())) {
    spdlog::warn("World gamemode was invalid");
    return false;
  }

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

    SDK::FDataTableRowHandle row_handle = {
        .DataTable = *data_table,
        .RowName = String::to_fname(item.item_name()),
    };

    spdlog::info("Giving '{}' x{} to player", item, count, player);

    game_mode->GrantItemsToPlayer(character, row_handle, count);

    return true;
  }

  return false;
}

auto PlayerManager::is_valid_player(PlayerHandle player) -> bool {
  auto handle = get_player_by_id(player.id());
  return handle.has_value();
}
