#pragma once

#include <string>
#include <unordered_map>

#include "G2/DataTableHandle.hpp"
#include "G2/ItemHandle.hpp"

struct DataTableInfo final {
  explicit DataTableInfo() {}
  explicit DataTableInfo(DataTableHandle handle,
                         std::unordered_map<std::string, ItemHandle> items)
      : m_handle(handle), m_items(items) {}

  auto handle() -> DataTableHandle & { return m_handle; }
  auto handle() const -> const DataTableHandle & { return m_handle; }

  auto items() -> std::unordered_map<std::string, ItemHandle> & {
    return m_items;
  }
  auto items() const -> const std::unordered_map<std::string, ItemHandle> & {
    return m_items;
  }

private:
  DataTableHandle m_handle;
  std::unordered_map<std::string, ItemHandle> m_items;
};
