#pragma once

#include <map>
#include <string>

#include "G2/DataTableHandle.hpp"
#include "G2/ItemInfo.hpp"

struct DataTable final {
  explicit DataTable() {}
  explicit DataTable(DataTableHandle handle,
                     std::map<std::string, ItemInfo> items)
      : m_handle(handle), m_items(items) {}

  auto handle() -> DataTableHandle & { return m_handle; }
  auto handle() const -> const DataTableHandle & { return m_handle; }

  auto name() -> std::string & { return handle().name(); }
  auto name() const -> const std::string & { return handle().name(); }

  auto items() -> std::map<std::string, ItemInfo> & { return m_items; }
  auto items() const -> const std::map<std::string, ItemInfo> & {
    return m_items;
  }

private:
  DataTableHandle m_handle;
  std::map<std::string, ItemInfo> m_items;
};
