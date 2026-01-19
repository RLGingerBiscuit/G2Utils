#include "UI/TableList.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/spdlog.h>

#include "G2/DataTableManager.hpp"
#include "defer.hpp"

auto TableList::refresh() -> void {
  DataTableManager::get().refresh();

  if (m_selected_table.has_value() &&
      !DataTableManager::get().is_valid_table(*m_selected_table)) {
    m_selected_table = {};
  }

  refilter_tables();
}

auto TableList::render() -> void {
  if (ImGui::BeginListBox("Table List")) {
    defer(ImGui::EndListBox());

    for (auto &handle : m_filtered_tables) {
      if (ImGui::Selectable(handle.name().c_str(),
                            m_selected_table.has_value() &&
                                *m_selected_table == handle)) {
        spdlog::debug("Selected table changed to {}", handle);
        m_selected_table = handle;
      }
    }
  }

  if (ImGui::InputText("Filter##TableList", &m_filter)) {
    refilter_tables();
    spdlog::info("Filtered tables: '{}'!", m_filter);
  }

  if (m_selected_table.has_value()) {
    ImGui::Text("Selected table: '%s'",
                fmt::to_string(*m_selected_table).c_str());
  } else {
    ImGui::Text("Selected table: None");
  }
}

auto TableList::refilter_tables() -> void {
  m_filtered_tables.clear();

  auto tables = DataTableManager::get().get_all_tables();

  if (m_filter == "") {
    m_filtered_tables.reserve(tables.size());
    for (auto &table : tables) {
      m_filtered_tables.push_back(table.handle());
    }
  } else {
    auto filter = m_filter;
    std::transform(filter.begin(), filter.end(), filter.begin(),
                   [](uint8_t c) { return std::tolower(c); });

    for (auto &table : tables) {
      auto table_name = table.name();
      std::transform(table_name.begin(), table_name.end(), table_name.begin(),
                     [](uint8_t c) { return std::tolower(c); });

      if (table_name.contains(filter))
        m_filtered_tables.push_back(table.handle());
    }
  }

  std::sort(m_filtered_tables.begin(), m_filtered_tables.end(),
            [](const DataTableHandle &x, const DataTableHandle &y) {
              return x.name() < y.name();
            });
}
