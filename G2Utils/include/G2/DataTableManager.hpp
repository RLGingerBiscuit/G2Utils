#pragma once

#include <optional>

#include "G2/DataTable.hpp"
#include "G2/DataTableHandle.hpp"
#include "G2/Singleton.hpp"

class DataTableManager final : public Singleton<DataTableManager> {
  friend class Singleton<DataTableManager>;

public:
  // Refreshes all data tables from the game.
  auto refresh() -> void;

  // Check whether the given DataTableHandle refers to a valid data table.
  //
  // NOTE: This does NOT check whether the data table currently exists in the
  // game, that is left to the caller, and should be done as late as possible
  // for performance reasons.
  auto is_valid_table(const DataTableHandle &handle) -> bool {
    return m_table_cache.contains(handle);
  }

  // Retrieves all cached data tables.
  auto get_all_tables() -> std::vector<DataTable>;

  // Retrieves extended information about the given data table from the cache.
  auto get_table_info(const DataTableHandle &handle)
      -> std::optional<DataTable>;

private:
  std::map<DataTableHandle, DataTable> m_table_cache;
};
