#pragma once

#include <optional>

#include "G2/DataTableManager.hpp"

class TableList final {
public:
  auto refresh() -> void;
  auto render() -> void;

  auto selected_table() -> std::optional<DataTableInfo> {
    return m_selected_table;
  }

private:
  std::unordered_map<std::string, DataTableHandle> m_tables;
  std::optional<DataTableInfo> m_selected_table;
};
