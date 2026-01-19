#include "Dump/DataTableDumper.hpp"

#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>

#include "G2/ConfigManager.hpp"
#include "G2/DataTableManager.hpp"

static auto dump_table(const DataTable &table) -> void {
  auto dump_path = ConfigManager::get().dump_path();
  std::filesystem::create_directories(dump_path);

  std::filesystem::path dump_file =
      dump_path / fmt::format("{}.md", table.name());
  auto stream = std::ofstream(dump_file);
  if (!stream.is_open()) {
    spdlog::error("Failed to open file for writing");
    return;
  }

  stream << "# Data Table Dump - " << table.name() << "\n\n";

  stream << "| Column Name | Display Name | Description |\n";
  stream << "|-------------|--------------|-------------|\n";

  for (const auto &[_, item] : table.items()) {
    stream << fmt::format("| {} | {} | {} |\n", item.name(),
                          item.display_name(), item.plain_description());
  }

  stream << "\n";

  stream.close();
}

DataTableDumper::DataTableDumper(const DataTableHandle &handle)
    : m_handle(handle) {}

auto DataTableDumper::dump() const -> void {
  auto maybe_table = DataTableManager::get().get_table_info(m_handle);
  if (!maybe_table) {
    spdlog::error("Table {} not found", m_handle.name());
    return;
  }

  spdlog::info("Dumping table {}", m_handle.name());
  dump_table(*maybe_table);
  spdlog::info("Dumped table {}", m_handle.name());
}

auto DataTableDumper::dump_all() -> void {
  spdlog::info("Dumping all tables");
  auto tables = DataTableManager::get().get_all_tables();
  for (const auto &table : tables) {
    dump_table(table);
  }
  spdlog::info("Dumped all tables");
}
