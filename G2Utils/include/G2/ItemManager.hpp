#pragma once

#include <optional>
#include <unordered_map>

#include "G2/ItemHandle.hpp"
#include "G2/ItemInfo.hpp"
#include "G2/Singleton.hpp"

class ItemManager final : public Singleton<ItemManager> {
  friend class Singleton<ItemManager>;

public:
  // Reloads all items from the cached data tables.
  //
  // This should be called after DataTableManager::refresh().
  auto refresh() -> void;

  // Check whether the given ItemHandle refers to a cached item.
  //
  // NOTE: This does NOT check whether the item currently exists in the
  // game, that is left to the caller, and should be done as late as possible
  // for performance reasons.
  auto is_valid_item(const ItemHandle &handle) -> bool {
    return m_item_cache.contains(handle);
  }

  // Retrieves all cached items.
  auto get_all_items() -> std::vector<ItemInfo>;

  // Retrieves extended information about the given item from the cache.
  auto get_item_info(const ItemHandle &handle) -> std::optional<ItemInfo>;

private:
  std::unordered_map<ItemHandle, ItemInfo> m_item_cache;
};
