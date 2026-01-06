#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Maine_structs.hpp>
#include <unordered_set>

#include "G2/DataTableHandle.hpp"
#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"
#include "G2/StringManager.hpp"
#include "SDK/SDK/OEIText_classes.hpp"

SINGLETON_IMPL(ItemManager);

auto ItemManager::init_singleton() -> void {}

auto ItemManager::deinit_singleton() -> void {}

auto ItemManager::is_valid_item(ItemHandle item) -> bool {
  // TODO: reduce copying
  auto table = DataTableManager::get().get_table_info(item.table_handle());
  if (!table.has_value())
    return false;

  return table->items().contains(item.item_name());
}

auto ItemManager::get_many_items(std::span<ItemHandle> span)
    -> std::vector<ItemInfo> {
  std::vector<ItemInfo> results;

  std::unordered_map<DataTableHandle, std::unordered_set<ItemHandle>> tables;

  for (auto &item : span) {
    tables.try_emplace(item.table_handle(), std::unordered_set<ItemHandle>())
        .first->second.emplace(item);
  }

  for (auto &[table_handle, items] : tables) {
    auto table = DataTableManager::get().get_table_raw(table_handle);
    if (!table.has_value())
      continue;

    const auto &map = table.value()->RowMap;

    for (const auto &row : map) {
      auto name = String::to_string(row.Key());
      ItemHandle row_handle(table_handle, name);
      if (!items.contains(row_handle))
        continue;

      auto *data = reinterpret_cast<SDK::FBaseItemData *>(row.Value());
      auto display_name = String::to_string(
          SDK::UOEITextLibrary::GetLocStringText(data->LocalizedDisplayName));

      ItemInfo info;
      if (display_name.starts_with("Invalid")) {
        // Doesn't exist, just use the internal name
        info = ItemInfo(row_handle, row_handle.item_name());
      } else {
        info = ItemInfo(row_handle, display_name);
      }
      m_info_cache[row_handle] = info;
      results.push_back(info);

      items.erase(row_handle);
    }
  }

  return results;
}

auto ItemManager::get_item(ItemHandle item) -> std::optional<ItemInfo> {
  // NOTE: the order of these two if statements is *VERY* important.
  // If an item is in the cache, it will have been validated before, so
  // we can skip expensive validation.
  if (m_info_cache.contains(item))
    return m_info_cache[item];

  if (!is_valid_item(item))
    return {};

  const auto table = DataTableManager::get().get_table_raw(item.table_handle());
  assert(table.has_value());

  const auto &map = table.value()->RowMap;

  for (const auto &row : map) {
    auto name = String::to_string(row.Key());
    if (name != item.item_name())
      continue;

    auto *data = reinterpret_cast<SDK::FBaseItemData *>(row.Value());
    auto display_name = String::to_string(
        SDK::UOEITextLibrary::GetLocStringText(data->LocalizedDisplayName));

    ItemInfo info;
    if (display_name.starts_with("Invalid")) {
      // Doesn't exist, just use the internal name
      info = ItemInfo(item, item.item_name());
    } else {
      info = ItemInfo(item, display_name);
    }
    m_info_cache[item] = info;
    return info;
  }

  return {};
}
