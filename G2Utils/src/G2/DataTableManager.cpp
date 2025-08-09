#include <cstdint>
#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include <SDK/SDK/Basic.hpp>
#include <SDK/SDK/CoreUObject_classes.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Maine_structs.hpp>

#include "G2/DataTableManager.hpp"

SINGLETON_IMPL(DataTableManager);

auto DataTableManager::init_singleton() -> void {}

auto DataTableManager::deinit_singleton() -> void {}

auto DataTableManager::get_all_tables()
    -> std::unordered_map<std::string, DataTableHandle> {
  std::unordered_map<std::string, DataTableHandle> tables;

  for (int32_t i = 0; i < SDK::UObject::GObjects->Num(); ++i) {
    auto *object = SDK::UObject::GObjects->GetByIndex(i);
    if (object == nullptr)
      continue;

    if (!object->IsA(SDK::UDataTable::StaticClass()))
      continue;

    auto *table = static_cast<SDK::UDataTable *>(object);

    // FIXME: This seems a wee bit too hacky
    if (table->RowStruct == nullptr ||
        !table->RowStruct->GetName().contains("BaseItemData"))
      continue;

    auto table_name = table->GetName();

    tables[table_name] = DataTableHandle(table_name);
  }

  return tables;
}

auto DataTableManager::get_table_raw(DataTableHandle handle)
    -> SDK::UDataTable * {
  // auto *object = SDK::UObject::FindObject(handle.name());
  SDK::UObject *object = nullptr;
  for (int i = 0; i < SDK::UObject::GObjects->Num(); ++i) {
    auto *obj = SDK::UObject::GObjects->GetByIndex(i);
    // NOTE: For whatever reason, data tables are also packages (sometimes?), so
    //       to use the short name (faster comparison), those (and everything
    //       else) are excluded.
    if (!obj || obj->Class->CastFlags != SDK::EClassCastFlags::None ||
        obj->GetName() != handle.name())
      continue;
    object = obj;
    break;
  }

  if (object == nullptr || !object->IsA(SDK::UDataTable::StaticClass()))
    return nullptr;

  return reinterpret_cast<SDK::UDataTable *>(object);
}

auto DataTableManager::get_table_info(DataTableHandle handle)
    -> std::optional<DataTableInfo> {
  auto table = get_table_raw(handle);
  if (table == nullptr)
    return {};

  std::unordered_map<std::string, ItemHandle> table_items;

  auto &map = table->RowMap;
  table_items.reserve(map.Num());

  for (const auto &row : map) {
    auto item_name = row.Key().ToString();
    auto *item_data = reinterpret_cast<SDK::FBaseItemData *>(row.Value());
    if (item_data == nullptr) {
      spdlog::warn("{}.{} is null", handle.name(), item_name);
      continue;
    }
    table_items[item_name] = ItemHandle(handle.name(), item_name);
  }

  return DataTableInfo(handle.name(), table_items);
}

auto DataTableManager::get_table_and_row(ItemHandle handle)
    -> std::pair<SDK::UDataTable *, SDK::FName> {
  auto table = get_table_raw(handle.table_handle());
  if (table == nullptr)
    return {};

  auto &map = table->RowMap;

  for (const auto &row : map) {
    auto name = row.Key().ToString();
    if (name == handle.item_name())
      return {table, row.Key()};
  }

  return {};
}