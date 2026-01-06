#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/spdlog.h>

#include "G2/DataTableManager.hpp"
#include "defer.hpp"

#include "UI/TableList.hpp"

auto TableList::refresh() -> void {
  auto new_tables = DataTableManager::get().get_all_tables();

  if (m_selected_table.has_value() &&
      !new_tables.contains(m_selected_table->name())) {
    m_selected_table = {};
  }

  m_tables = new_tables;
  refilter_tables();
}

auto TableList::render() -> void {
  if (ImGui::BeginListBox("Table List")) {
    defer(ImGui::EndListBox());

    for (auto &table : m_filtered_tables) {
      if (ImGui::Selectable(table.name().c_str(),
                            m_selected_table.has_value() &&
                                m_selected_table->name() == table.name())) {
        spdlog::info("Selected table changed to {}", table);
        m_selected_table = DataTableManager::get().get_table_info(
            DataTableHandle(table.name()));
      }
    }
  }

  if (ImGui::InputText("Filter##TableList", &m_filter)) {
    refilter_tables();
    spdlog::info("Filtered tables: '{}'!", m_filter);
  }

  if (m_selected_table.has_value()) {
    ImGui::Text("Selected table: '%s'",
                fmt::to_string(m_selected_table->to_handle()).c_str());
  } else {
    ImGui::Text("Selected table: None");
  }
}

auto TableList::refilter_tables() -> void {
  m_filtered_tables.clear();
  if (m_filter == "") {
    m_filtered_tables.reserve(m_tables.size());
    for (auto &pair : m_tables) {
      m_filtered_tables.push_back(pair.second);
    }
  } else {
    auto filter = m_filter;
    std::transform(filter.begin(), filter.end(), filter.begin(),
                   [](uint8_t c) { return std::tolower(c); });

    for (auto &pair : m_tables) {
      auto table_name = pair.first;
      std::transform(table_name.begin(), table_name.end(), table_name.begin(),
                     [](uint8_t c) { return std::tolower(c); });
      if (table_name.contains(filter))
        m_filtered_tables.push_back(pair.second);
    }
  }

  std::sort(
      m_filtered_tables.begin(), m_filtered_tables.end(),
      [](DataTableHandle x, DataTableHandle y) { return x.name() < y.name(); });
}
