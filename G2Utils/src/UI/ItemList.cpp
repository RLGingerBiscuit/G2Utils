#include "UI/ItemList.hpp"

#include <algorithm>
#include <ranges>
#include <string>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "G2/ItemInfo.hpp"
#include "G2/ItemManager.hpp"
#include "defer.hpp"

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

  m_prev_selected_table = selected_table->handle();
  refilter_items();
}

auto ItemList::render() -> void {
  auto table = m_table_list.selected_table();

  if (ImGui::BeginListBox("Item List")) {
    defer(ImGui::EndListBox());

    if (table.has_value()) {
      if (table->handle() != m_prev_selected_table)
        refresh();

      for (auto &item : m_filtered_items) {
        if (ImGui::Selectable(item.display_name().c_str(),
                              m_selected_item.has_value() &&
                                  *m_selected_item == item.handle())) {
          spdlog::info("Selected item changed to {}", item.name());
          m_selected_item = item.handle();
        }
      }
    }
  }

  if (ImGui::InputText("Filter##ItemList", &m_filter))
    refilter_items();

  if (m_selected_item.has_value()) {
    auto info = ItemManager::get().get_item(*m_selected_item);
    ImGui::Text("Selected item: '%s'", info->display_name().c_str());
  } else {
    ImGui::Text("Selected item: None");
  }
}

auto ItemList::refilter_items() -> void {
  m_filtered_items.clear();
  auto selected_table = m_table_list.selected_table();
  if (!selected_table.has_value())
    return;

  auto items = selected_table->items() | std::views::values |
               std::ranges::to<std::vector>();
  auto unfiltered = ItemManager::get().get_many_items(items);

  std::vector<ItemInfo> filtered;

  if (m_filter == "") {
    filtered.append_range(unfiltered);
  } else {
    auto filter = m_filter;
    std::transform(filter.begin(), filter.end(), filter.begin(),
                   [](uint8_t c) { return std::tolower(c); });

    for (auto &item : unfiltered) {
      auto item_name = item.display_name();
      std::transform(item_name.begin(), item_name.end(), item_name.begin(),
                     [](uint8_t c) { return std::tolower(c); });
      if (item_name.contains(filter))
        filtered.push_back(item);
    }
  }

  m_filtered_items = std::move(filtered);
}
