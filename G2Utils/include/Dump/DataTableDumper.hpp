#pragma once

#include "G2/DataTableHandle.hpp"

class DataTableDumper final {
public:
  DataTableDumper(const DataTableHandle &handle);

  // Dumps the contents of the DataTable to a markdown file.
  auto dump() const -> void;

  // Dumps the contents of all DataTables to markdown files.
  static auto dump_all() -> void;

private:
  const DataTableHandle &m_handle;
};
