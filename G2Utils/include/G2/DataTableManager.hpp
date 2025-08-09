#pragma once

#include "G2/ItemManager.hpp"
#include "G2/Singleton.hpp"
#include <optional>

struct DataTableHandle final {
  explicit DataTableHandle() {}
  explicit DataTableHandle(std::string name) : m_name(name) {}

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

private:
  std::string m_name;
};

struct DataTableInfo final {
  explicit DataTableInfo() {}
  explicit DataTableInfo(std::string name,
                         std::unordered_map<std::string, ItemHandle> items)
      : m_name(name), m_items(items) {}

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

  auto items() -> std::unordered_map<std::string, ItemHandle> & {
    return m_items;
  }
  auto items() const -> const std::unordered_map<std::string, ItemHandle> & {
    return m_items;
  }

  auto to_handle() -> DataTableHandle { return DataTableHandle(m_name); }

private:
  std::string m_name;
  std::unordered_map<std::string, ItemHandle> m_items;
};

class DataTableManager final : public Singleton<DataTableManager> {
  friend class Singleton<DataTableManager>;

public:
  auto get_all_tables() -> std::unordered_map<std::string, DataTableHandle>;

  auto get_table_info(DataTableHandle handle) -> std::optional<DataTableInfo>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
