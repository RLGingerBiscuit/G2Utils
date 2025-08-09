#pragma once

#include <string>
#include <unordered_map>

#include "G2/DataTableHandle.hpp"
#include "G2/ItemHandle.hpp"

struct DataTableInfo final {
  explicit DataTableInfo() {}
  explicit DataTableInfo(std::string name,
                         std::unordered_map<std::string, ItemHandle> items)
      : m_name(name), m_items(items) {}

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

  auto items() -> std::unordered_map<std::string, ItemHandle> & {
    return m_items;
  }
  auto items() const -> const std::unordered_map<std::string, ItemHandle> & {
    return m_items;
  }

  auto to_handle() -> DataTableHandle { return DataTableHandle(m_name); }

private:
  std::string m_name;
  std::unordered_map<std::string, ItemHandle> m_items;
};