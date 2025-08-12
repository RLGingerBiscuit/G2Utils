#pragma once

#include <optional>

#include "G2/DataTableHandle.hpp"
#include "G2/DataTableInfo.hpp"

class TableList final {
public:
  auto render() -> void;
  auto refresh() -> void;

  auto has_inited_tables() -> bool { return m_tables.size() > 0; }

  auto selected_table() -> std::optional<DataTableInfo> {
    return m_selected_table;
  }

private:
  auto refilter_tables() -> void;

private:
  std::unordered_map<std::string, DataTableHandle> m_tables;
  std::optional<DataTableInfo> m_selected_table;

  std::string m_filter;
  std::vector<DataTableHandle> m_filtered_tables;
};
