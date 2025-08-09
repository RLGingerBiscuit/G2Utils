#include <imgui.h>
#include <spdlog/spdlog.h>

#include "G2/DataTableManager.hpp"
#include "defer.hpp"

#include "UI/TableList.hpp"

auto TableList::refresh() -> void {
  auto new_tables = DataTableManager::instance().get_all_tables();

  if (m_selected_table.has_value() &&
      !new_tables.contains(m_selected_table->name())) {
    m_selected_table = {};
  }

  m_tables = new_tables;
}

auto TableList::render() -> void {
  if (ImGui::BeginListBox("Table List")) {
    defer(ImGui::EndListBox());

    for (auto &pair : m_tables) {
      auto &table = pair.second;

      if (ImGui::Selectable(table.name().c_str(),
                            m_selected_table.has_value() &&
                                m_selected_table->name() == table.name())) {
        m_selected_table = DataTableManager::instance().get_table_info(
            DataTableHandle(table.name()));
      }
    }
  }

  if (m_selected_table.has_value()) {
    auto &selected_table = *m_selected_table;
    ImGui::Text("Selected table: '%s'", selected_table.name().c_str());
  } else {
    ImGui::Text("Selected table: None");
  }
}
