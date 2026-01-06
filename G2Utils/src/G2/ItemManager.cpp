#include "G2/ItemManager.hpp"

#include <ranges>
#include <unordered_set>

#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/Maine_structs.hpp>
#include <SDK/SDK/OEIText_classes.hpp>

#include "G2/DataTableManager.hpp"
#include "G2/ItemHandle.hpp"

SINGLETON_IMPL(ItemManager);

auto ItemManager::refresh() -> void {
  m_item_cache.clear();

  // Counts unique item names
  std::unordered_set<std::string> names_for_total;

  auto &man = DataTableManager::get();
  for (const auto table : man.get_all_tables()) {
    for (const auto &[_, item] : table.items()) {
      m_item_cache.emplace(item.handle(), item);
      names_for_total.insert(item.name());
    }
  }

  spdlog::info("ItemManager: Cached {} items", names_for_total.size());
}

auto ItemManager::get_all_items() -> std::vector<ItemInfo> {
  return m_item_cache | std::views::values | std::ranges::to<std::vector>();
}

auto ItemManager::get_item_info(const ItemHandle &handle)
    -> std::optional<ItemInfo> {
  auto it = m_item_cache.find(handle);
  if (it != m_item_cache.end()) {
    return it->second;
  }
  return std::nullopt;
}
