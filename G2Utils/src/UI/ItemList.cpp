#include <imgui.h>
#include <spdlog/spdlog.h>

#include "G2/DataTableManager.hpp"
#include "defer.hpp"

#include "UI/ItemList.hpp"

auto ItemList::refresh() -> void {
  // TODO: try reduce the copying going on (shared_ptr ftw?)
  auto selected_table = m_table_list.selected_table();

  if (!selected_table.has_value()) {
    m_selected_item = {};
    m_prev_selected_table = {};
    return;
  }

  auto &new_items = selected_table.value().items();

  if (m_selected_item.has_value() &&
      !new_items.contains(m_selected_item->item_name())) {
    m_selected_item = {};
  }

  m_prev_selected_table = selected_table->name();
}

auto ItemList::render() -> void {
  auto table = m_table_list.selected_table();

  if (ImGui::BeginListBox("Item List")) {
    defer(ImGui::EndListBox());

    if (table.has_value()) {
      if (table->name() != m_prev_selected_table)
        refresh();

      auto &items = table->items();

      for (auto &pair : items) {
        auto &item = pair.second;

        if (ImGui::Selectable(item.item_name().c_str(),
                              m_selected_item.has_value() &&
                                  m_selected_item->item_name() ==
                                      item.item_name())) {
          spdlog::info("Selected item changed to {}", item.item_name());
          m_selected_item = item;
        }
      }
    }
  }

  if (m_selected_item.has_value()) {
    auto &selected_item = *m_selected_item;
    ImGui::Text("Selected item: '%s'", selected_item.item_name().c_str());
  } else {
    ImGui::Text("Selected item: None");
  }
}
