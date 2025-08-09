#pragma once

#include <string>

#include "G2/DataTableHandle.hpp"

struct ItemHandle final {
  explicit ItemHandle() {}
  explicit ItemHandle(std::string table_name, std::string item_name)
      : m_table_name(table_name), m_item_name(item_name) {}

  auto table_name() -> std::string & { return m_table_name; }
  auto table_name() const -> const std::string & { return m_table_name; }

  auto item_name() -> std::string & { return m_item_name; }
  auto item_name() const -> const std::string & { return m_item_name; }

  auto table_handle() -> DataTableHandle {
    return DataTableHandle(m_table_name);
  }

private:
  std::string m_table_name;
  std::string m_item_name;
};
