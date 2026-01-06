#pragma once

#include <optional>

#include "G2/DataTableHandle.hpp"
#include "G2/DataTableInfo.hpp"
#include "G2/Singleton.hpp"

// FIXME: Preferably, I want to keep all mentions of the SDK functions to the
//        .cpp files. This ain't that.
namespace SDK {
class UDataTable;
} // namespace SDK

class DataTableManager final : public Singleton<DataTableManager> {
  friend class Singleton<DataTableManager>;

public:
  auto get_all_tables() -> std::unordered_map<std::string, DataTableHandle>;

  auto get_table_raw(DataTableHandle handle)
      -> std::optional<SDK::UDataTable *>;

  auto get_table_info(DataTableHandle handle) -> std::optional<DataTableInfo>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
