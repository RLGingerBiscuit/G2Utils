#pragma once

#include "G2/ItemHandle.hpp"
#include "G2/Singleton.hpp"

class ItemManager final : public Singleton<ItemManager> {
  friend class Singleton<ItemManager>;

public:
  auto is_valid_item(ItemHandle item) -> bool;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
