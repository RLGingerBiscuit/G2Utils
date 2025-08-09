#pragma once

#include <string>

#include "G2/Singleton.hpp"

struct ItemHandle final {
  explicit ItemHandle() {}
  explicit ItemHandle(std::string table_name, std::string item_name)
      : m_table_name(table_name), m_item_name(item_name) {}

  auto table_name() -> std::string & { return m_table_name; }
  auto table_name() const -> const std::string & { return m_table_name; }

  auto item_name() -> std::string & { return m_item_name; }
  auto item_name() const -> const std::string & { return m_item_name; }

private:
  std::string m_table_name;
  std::string m_item_name;
};

// struct ItemInfo final {
//   explicit ItemInfo() {}
//   explicit ItemInfo(std::string table_name, std::string name,
//                     std::string display_name)
//       : m_table_name(table_name), m_name(name),
//         m_display_name(display_name) {}

//   auto table_name() -> std::string & { return m_table_name; }
//   auto table_name() const -> const std::string & { return m_table_name; }

//   auto name() -> std::string & { return m_name; }
//   auto name() const -> const std::string & { return m_name; }

//   auto display_name() -> std::string & { return m_display_name; }
//   auto display_name() const -> const std::string & { return m_display_name; }

//   auto to_handle() -> ItemHandle {
//     return ItemHandle(m_name, m_table_name);
//   }

// private:
//   std::string m_table_name;
//   std::string m_name;
//   std::string m_display_name;
// };

class ItemManager final : public Singleton<ItemManager> {
  friend class Singleton<ItemManager>;

public:
  // auto get_item_info(ItemHandle handle) -> std::optional<ItemInfo>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
