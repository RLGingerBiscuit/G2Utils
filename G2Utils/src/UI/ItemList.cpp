#include <algorithm>
#include <cctype>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/spdlog.h>

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

  m_prev_selected_table = selected_table->to_handle();
  refilter_items();
}

auto ItemList::render() -> void {
  auto table = m_table_list.selected_table();

  if (ImGui::BeginListBox("Item List")) {
    defer(ImGui::EndListBox());

    if (table.has_value()) {
      if (table->to_handle() != m_prev_selected_table)
        refresh();

      for (auto &item : m_filtered_items) {
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

  if (ImGui::InputText("Filter##ItemList", &m_filter))
    refilter_items();

  if (m_selected_item.has_value()) {
    auto &selected_item = *m_selected_item;
    ImGui::Text("Selected item: '%s'", selected_item.item_name().c_str());
  } else {
    ImGui::Text("Selected item: None");
  }
}

auto ItemList::refilter_items() -> void {
  m_filtered_items.clear();
  auto selected_table = m_table_list.selected_table();
  if (!selected_table.has_value())
    return;

  auto &items = selected_table->items();

  if (m_filter == "") {
    m_filtered_items.reserve(items.size());
    for (auto &pair : items) {
      m_filtered_items.push_back(pair.second);
    }
  } else {
    auto filter = m_filter;
    std::transform(filter.begin(), filter.end(), filter.begin(),
                   [](uint8_t c) { return std::tolower(c); });

    for (auto &pair : items) {
      auto item_name = pair.first;
      std::transform(item_name.begin(), item_name.end(), item_name.begin(),
                     [](uint8_t c) { return std::tolower(c); });
      if (item_name.contains(filter))
        m_filtered_items.push_back(pair.second);
    }
  }

  std::sort(
      m_filtered_items.begin(), m_filtered_items.end(),
      [](ItemHandle x, ItemHandle y) { return x.item_name() < y.item_name(); });
}
