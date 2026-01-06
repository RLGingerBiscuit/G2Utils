#pragma once

#include <optional>
#include <span>
#include <unordered_map>

#include "G2/ItemHandle.hpp"
#include "G2/ItemInfo.hpp"
#include "G2/Singleton.hpp"

class ItemManager final : public Singleton<ItemManager> {
  friend class Singleton<ItemManager>;

public:
  // Check whether the given ItemHandle refers to a valid item.
  //
  // NOTE: This validation is very slow, so avoid calling this in tight loops.
  // Prefer caching results or using get_many_items when validating multiple
  // handles.
  auto is_valid_item(ItemHandle item) -> bool;

  // Retrieve ItemInfo for a sequence of ItemHandles.
  //
  // This is a convenience/bulk operation that will return a vector of
  // ItemInfo objects corresponding to the provided handles. Invalid handles
  // are skipped. Because individual validation (is_valid_item) is
  // very slow, using this bulk method can be more efficient than repeatedly
  // calling get_item in a loop.
  auto get_many_items(std::span<ItemHandle> items) -> std::vector<ItemInfo>;

  // Retrieve ItemInfo for a single ItemHandle.
  //
  // Returns std::nullopt if the handle is invalid or the item cannot be
  // resolved. Validation performed here is very slow; callers should avoid
  // frequent calls and consider caching results or batching requests via
  // get_many_items.
  auto get_item(ItemHandle item) -> std::optional<ItemInfo>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;

private:
  std::unordered_map<ItemHandle, ItemInfo> m_info_cache;
};
