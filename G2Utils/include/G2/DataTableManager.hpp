#pragma once

#include <optional>
#include <utility>

#include "G2/DataTableHandle.hpp"
#include "G2/DataTableInfo.hpp"
#include "G2/ItemHandle.hpp"
#include "G2/Singleton.hpp"

// FIXME: Preferably, I want to keep all mentions of the SDK functions to the
//        .cpp files. This ain't that.
namespace SDK {
class UDataTable;
class FName;
} // namespace SDK

class DataTableManager final : public Singleton<DataTableManager> {
  friend class Singleton<DataTableManager>;

public:
  auto get_all_tables() -> std::unordered_map<std::string, DataTableHandle>;

  auto get_table_raw(DataTableHandle handle) -> SDK::UDataTable *;

  auto get_table_info(DataTableHandle handle) -> std::optional<DataTableInfo>;

  auto get_table_and_row(ItemHandle item)
      -> std::pair<SDK::UDataTable *, SDK::FName>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
