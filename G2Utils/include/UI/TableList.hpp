#pragma once

#include <optional>
#include <vector>

#include "G2/DataTableHandle.hpp"

class TableList final {
public:
  auto render() -> void;
  auto refresh() -> void;

  auto selected_table() -> std::optional<DataTableHandle> {
    return m_selected_table;
  }

private:
  auto refilter_tables() -> void;

private:
  std::optional<DataTableHandle> m_selected_table;

  std::string m_filter;
  std::vector<DataTableHandle> m_filtered_tables;
};
