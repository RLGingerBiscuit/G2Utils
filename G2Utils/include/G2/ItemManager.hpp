#pragma once

#include <cstdint>

#include "G2/ItemHandle.hpp"
#include "G2/PlayerHandle.hpp"
#include "G2/Singleton.hpp"

// struct ItemInfo final {
//   explicit ItemInfo() {}
//   explicit ItemInfo(std::string table_name, std::string name,
//                     std::string display_name)
//       : m_table_name(table_name), m_name(name), m_display_name(display_name)
//       {}

//   auto table_name() -> std::string & { return m_table_name; }
//   auto table_name() const -> const std::string & { return m_table_name; }

//   auto name() -> std::string & { return m_name; }
//   auto name() const -> const std::string & { return m_name; }

//   auto display_name() -> std::string & { return m_display_name; }
//   auto display_name() const -> const std::string & { return m_display_name; }

//   auto to_handle() -> ItemHandle { return ItemHandle(m_name, m_table_name); }

// private:
//   std::string m_table_name;
//   std::string m_name;
//   std::string m_display_name;
// };

class ItemManager final : public Singleton<ItemManager> {
  friend class Singleton<ItemManager>;

public:
  // auto get_item_info(ItemHandle handle) -> std::optional<ItemInfo>;

  auto is_valid_item(ItemHandle item) -> bool;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
