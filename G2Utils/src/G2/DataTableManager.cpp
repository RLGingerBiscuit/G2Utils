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
    -> std::optional<SDK::UDataTable *> {
  SDK::UDataTable *table = nullptr;

  for (int i = 0; i < SDK::UObject::GObjects->Num(); ++i) {
    auto *obj = SDK::UObject::GObjects->GetByIndex(i);
    if (!obj || !obj->IsA(SDK::UDataTable::StaticClass()) ||
        obj->GetName() != handle.name())
      continue;
    table = reinterpret_cast<SDK::UDataTable *>(obj);
    break;
  }

  if (table == nullptr) {
    spdlog::error("Data Table '{}' could not be found", handle);
    return {};
  }

  return table;
}

auto DataTableManager::get_table_info(DataTableHandle handle)
    -> std::optional<DataTableInfo> {
  auto table = get_table_raw(handle);
  if (!table.has_value())
    return {};

  std::unordered_map<std::string, ItemHandle> table_items;

  auto &map = table.value()->RowMap;
  table_items.reserve(map.Num());

  for (const auto &row : map) {
    auto item_name = row.Key().ToString();
    auto *item_data = reinterpret_cast<SDK::FBaseItemData *>(row.Value());
    auto item_handle = ItemHandle(handle.name(), item_name);
    if (item_data == nullptr) {
      spdlog::warn("'{}' is null", item_handle);
      continue;
    }
    table_items[item_name] = std::move(item_handle);
  }

  return DataTableInfo(handle.name(), table_items);
}
