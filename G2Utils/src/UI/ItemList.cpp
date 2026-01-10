#include "UI/ItemList.hpp"

#include <algorithm>
#include <ranges>
#include <string>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "G2/DataTableManager.hpp"
#include "G2/ItemInfo.hpp"
#include "G2/ItemManager.hpp"
#include "G2/ThemeManager.hpp"
#include "UI/RichText.hpp"
#include "defer.hpp"

auto ItemList::refresh() -> void {
  // TODO: Only want to refresh items when data tables have changed
  ItemManager::get().refresh();

  auto table = m_table_list.selected_table();
  m_prev_selected_table = table;

  if (!table.has_value()) {
    m_filtered_items.clear();
    m_selected_item = std::nullopt;
    return;
  }

  if (m_selected_item.has_value() && m_selected_item->table() != *table) {
    m_selected_item = std::nullopt;
  }

  refilter_items();
}

auto ItemList::render() -> void {
  auto table = m_table_list.selected_table();

  if (ImGui::BeginListBox("Item List")) {
    defer(ImGui::EndListBox());

    if (table.has_value()) {
      if (*table != m_prev_selected_table)
        refresh();

      for (auto &handle : m_filtered_items) {
        auto info = ItemManager::get().get_item_info(handle);
        assert(info.has_value());
        if (ImGui::Selectable(
                fmt::format("{}##{}", info->display_name(), info->name())
                    .c_str(),
                m_selected_item.has_value() && *m_selected_item == handle)) {
          spdlog::info("Selected item changed to {}", handle.name());
          m_selected_item = handle;
        }

        if (ImGui::BeginItemTooltip()) {
          defer(ImGui::EndTooltip());

          UI::OEIRichText(info->description().c_str(),
                          ThemeManager::get().theme());
        }
      }
    }
  }

  if (ImGui::InputText("Filter##ItemList", &m_filter))
    refilter_items();

  if (m_selected_item.has_value()) {
    auto info = ItemManager::get().get_item_info(*m_selected_item);

    switch (info->tier()) {
    case 1:
      ImGui::Text("I");
      break;
    case 2:
      ImGui::Text("II");
      break;
    case 3:
      ImGui::Text("III");
      break;
    case 4:
      ImGui::Text("IV");
      break;
    case 5:
      ImGui::Text("V");
      break;
    }

    if (info->tier() > 0)
      ImGui::SameLine();

    ImGui::Text("%s", info->display_name().c_str());

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text,
                          ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
    ImGui::Text(" (%s)", info->name().c_str());
    ImGui::PopStyleColor();

    UI::OEIRichText(info->description().c_str(), ThemeManager::get().theme());

    // TODO? Damage types/status effects/literally every stat???
  } else {
    ImGui::Text("Selected item: None");
  }
}

auto ItemList::refilter_items() -> void {
  m_filtered_items.clear();

  auto selected_table = m_table_list.selected_table();
  if (!selected_table.has_value())
    return;

  auto table = DataTableManager::get().get_table_info(*selected_table);
  if (!table.has_value())
    return;

  auto &items = table->items();

  if (m_filter == "") {
    m_filtered_items.reserve(items.size());
    for (auto &[_, item] : items) {
      m_filtered_items.push_back(item.handle());
    }
  } else {
    auto filter = m_filter;
    std::transform(filter.begin(), filter.end(), filter.begin(),
                   [](uint8_t c) { return std::tolower(c); });

    for (auto &[_, item] : items) {
      auto name = item.display_name();
      std::transform(name.begin(), name.end(), name.begin(),
                     [](uint8_t c) { return std::tolower(c); });

      if (name.contains(filter))
        m_filtered_items.push_back(item.handle());
    }
  }

  std::sort(m_filtered_items.begin(), m_filtered_items.end(),
            [](const ItemHandle &x, const ItemHandle &y) {
              return x.name() < y.name();
            });
}
