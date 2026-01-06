#pragma once

#include <optional>

#include "G2/ItemHandle.hpp"

#include "G2/ItemInfo.hpp"
#include "UI/TableList.hpp"

class ItemList final {
public:
  ItemList(TableList &table_list) : m_table_list(table_list) {}

  auto refresh() -> void;
  auto render() -> void;

  auto selected_item() -> std::optional<ItemHandle> { return m_selected_item; }

private:
  auto refilter_items() -> void;

private:
  std::optional<ItemHandle> m_selected_item;

  // Duplicate so we can detect when table changes
  std::optional<DataTableHandle> m_prev_selected_table;
  TableList &m_table_list;

  std::string m_filter;
  std::vector<ItemInfo> m_filtered_items;
};
