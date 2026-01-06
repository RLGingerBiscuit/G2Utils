#include "G2/PlayerManager.hpp"

#include <cstdint>

#include <SDK/SDK/BP_SurvivalGameMode_classes.hpp>
#include <SDK/SDK/BP_SurvivalPlayerCharacter_classes.hpp>
#include <SDK/SDK/BP_SurvivalPlayerState_classes.hpp>
#include <SDK/SDK/Basic.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Engine_structs.hpp>
#include <SDK/SDK/Maine_structs.hpp>

#include "G2/DataTableHandle.hpp"
#include "G2/StringManager.hpp"

SINGLETON_IMPL(PlayerManager);

auto get_data_table(const DataTableHandle &handle)
    -> std::optional<SDK::UDataTable *> {
  // FIXME: Don't really want this here, but also don't want to expose
  // UDataTable.
  SDK::UDataTable *table = nullptr;
  for (int32_t i = 0; i < SDK::UObject::GObjects->Num(); ++i) {
    // Wish I could use FindObjectFast, but that gets the package and not the
    // *actual* table.
    auto *object = SDK::UObject::GObjects->GetByIndex(i);
    if (object == nullptr || !object->IsA(SDK::UDataTable::StaticClass()) ||
        object->GetName() != handle.name())
      continue;

    table = static_cast<SDK::UDataTable *>(object);
    assert(table->RowStruct != nullptr);
    assert(table->RowStruct->GetName().contains("BaseItemData"));
  }

  if (table == nullptr)
    return std::nullopt;

  return table;
}

auto PlayerManager::refresh() -> void {
  m_player_cache.clear();

  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr) {
    spdlog::warn("World was null");
    return;
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return;
  }

  for (auto *player_state : state->PlayerArray) {
    if (player_state == nullptr)
      continue;
    auto player_name = String::to_string(player_state->GetPlayerName());
    PlayerHandle handle{player_state->PlayerId, player_name};
    m_player_cache.push_back(handle);
  }

  spdlog::info("PlayerManager: Cached {} players", m_player_cache.size());
}

auto PlayerManager::is_valid_player(const PlayerHandle &handle) -> bool {
  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr) {
    spdlog::warn("World was null");
    return false;
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return false;
  }

  for (auto *player_state : state->PlayerArray) {
    if (player_state == nullptr)
      continue;
    if (player_state->PlayerId == handle.id()) {
      return true;
    }
  }

  return false;
}

auto PlayerManager::get_player_by(
    std::function<bool(const PlayerHandle &)> predicate)
    -> std::optional<PlayerHandle> {
  for (const auto &player : m_player_cache) {
    if (predicate(player)) {
      return player;
    }
  }
  return std::nullopt;
}

auto PlayerManager::get_player_by_id(int32_t id)
    -> std::optional<PlayerHandle> {
  return get_player_by(
      [id](const PlayerHandle &player) { return player.id() == id; });
}

auto PlayerManager::get_player_by_name(const std::string &name)
    -> std::optional<PlayerHandle> {
  return get_player_by(
      [&name](const PlayerHandle &player) { return player.name() == name; });
}

auto PlayerManager::get_all_players() -> std::vector<PlayerHandle> {
  return m_player_cache;
}

auto PlayerManager::get_all_players_by(
    std::function<bool(const PlayerHandle &)> predicate)
    -> std::vector<PlayerHandle> {
  std::vector<PlayerHandle> result;
  for (const auto &player : m_player_cache) {
    if (predicate(player)) {
      result.push_back(player);
    }
  }
  return result;
}

auto PlayerManager::give_item_to_player(const PlayerHandle &player,
                                        const ItemHandle &item, int count)
    -> bool {
  auto *world = SDK::UWorld::GetWorld();
  if (world == nullptr) {
    spdlog::warn("World was null");
    return false;
  }

  auto *state = world->GameState;
  if (state == nullptr) {
    spdlog::warn("World state was null");
    return false;
  }

  auto data_table = get_data_table(item.table());
  if (!data_table.has_value()) {
    spdlog::warn("Could not find data table for item {}", item);
    return false;
  }

  SDK::ABP_SurvivalPlayerState_C *target_player_state = nullptr;
  for (auto *player_state : state->PlayerArray) {
    if (player_state == nullptr)
      continue;
    if (player_state->PlayerId == player.id()) {
      target_player_state =
          static_cast<SDK::ABP_SurvivalPlayerState_C *>(player_state);
      break;
    }
  }

  if (target_player_state == nullptr) {
    spdlog::warn("Could not find player state for player {}", player);
    return false;
  }

  auto *game_mode_base = world->AuthorityGameMode;
  if (game_mode_base == nullptr ||
      !game_mode_base->IsA(SDK::ABP_SurvivalGameMode_C::StaticClass())) {
    spdlog::warn("World gamemode was invalid");
    return false;
  }

  auto *game_mode = static_cast<SDK::ABP_SurvivalGameMode_C *>(game_mode_base);

  for (auto *player_state : state->PlayerArray) {
    if (player_state == nullptr || player_state->PlayerId != player.id())
      continue;

    auto *pawn = player_state->PawnPrivate;
    if (pawn == nullptr ||
        !pawn->IsA(SDK::ABP_SurvivalPlayerCharacter_C::StaticClass()))
      continue;

    auto *character = static_cast<SDK::ABP_SurvivalPlayerCharacter_C *>(pawn);

    SDK::FDataTableRowHandle row_handle = {
        .DataTable = *data_table,
        .RowName = String::to_fname(item.name()),
    };

    game_mode->GrantItemsToPlayer(character, row_handle, count);

    spdlog::info("Gave {} x{} to {}", item, count, player);

    return true;
  }

  return false;
}
