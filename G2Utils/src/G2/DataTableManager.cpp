#include "G2/DataTableManager.hpp"

#include <cstdint>
#include <ranges>
#include <string>
#include <map>

#include <spdlog/spdlog.h>

#include <SDK/SDK/Basic.hpp>
#include <SDK/SDK/CoreUObject_classes.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Maine_structs.hpp>

#include "G2/StringManager.hpp"

SINGLETON_IMPL(DataTableManager);

auto DataTableManager::refresh() -> void {
  m_table_cache.clear();

  for (int32_t i = 0; i < SDK::UObject::GObjects->Num(); ++i) {
    auto *object = SDK::UObject::GObjects->GetByIndex(i);
    if (object == nullptr || !object->IsA(SDK::UDataTable::StaticClass()))
      continue;

    auto *table = static_cast<SDK::UDataTable *>(object);

    if (table->RowStruct == nullptr ||
        !table->RowStruct->GetName().contains("BaseItemData"))
      continue;

    auto table_name = String::to_string(table->Name);
    DataTableHandle table_handle(table_name);

    auto &map = table->RowMap;

    std::map<std::string, ItemInfo> table_items;

    for (const auto &row : map) {
      auto item_name = String::to_string(row.Key());
      auto *item_data = reinterpret_cast<SDK::FBaseItemData *>(row.Value());
      auto item_handle = ItemHandle(table_handle, item_name);
      if (item_data == nullptr) {
        spdlog::warn("'{}' is null", item_handle);
        continue;
      }

      auto &equippable_data = item_data->EquippableData;

      auto display_name =
          String::from_locstring(item_data->LocalizedDisplayName);
      if (display_name.starts_with("Invalid"))
        display_name = item_name;

      auto description =
          String::from_locstring(item_data->LocalizedDescription);

      auto tier = item_data->Tier;

      table_items[item_name] =
          ItemInfo(item_handle, display_name, description, tier);
    }

    m_table_cache[table_handle] =
        DataTable(table_handle, std::move(table_items));
  }

  spdlog::trace("DataTableManager: Cached {} tables", m_table_cache.size());
}

auto DataTableManager::get_all_tables() -> std::vector<DataTable> {
  return m_table_cache | std::views::values | std::ranges::to<std::vector>();
}

auto DataTableManager::get_table_info(const DataTableHandle &handle)
    -> std::optional<DataTable> {
  auto it = m_table_cache.find(handle);
  if (it == m_table_cache.end())
    return std::nullopt;
  return it->second;
}
