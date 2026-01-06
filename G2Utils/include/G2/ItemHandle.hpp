#pragma once

#include <functional>
#include <string>

#include <spdlog/fmt/fmt.h>

#include "G2/DataTableHandle.hpp"

struct ItemHandle final {
  explicit ItemHandle() {}
  explicit ItemHandle(DataTableHandle table_handle, std::string item_name)
      : m_table_handle(table_handle), m_item_name(item_name) {}

  auto item_name() -> std::string & { return m_item_name; }
  auto item_name() const -> const std::string & { return m_item_name; }

  auto table_handle() -> DataTableHandle & { return m_table_handle; }
  auto table_handle() const -> const DataTableHandle & {
    return m_table_handle;
  }

  auto operator==(const ItemHandle &x) -> bool {
    return m_table_handle == x.m_table_handle && m_item_name == x.m_item_name;
  }
  auto operator==(const ItemHandle &x) const -> bool {
    return m_table_handle == x.m_table_handle && m_item_name == x.m_item_name;
  }

  auto operator!=(const ItemHandle &x) -> bool { return !(*this == x); }
  auto operator!=(const ItemHandle &x) const -> bool { return !(*this == x); }

private:
  DataTableHandle m_table_handle;
  std::string m_item_name;
};

template <>
struct fmt::formatter<ItemHandle> : fmt::formatter<fmt::string_view> {
  auto format(ItemHandle handle, fmt::format_context &ctx) const
      -> fmt::format_context::iterator;
};

namespace std {
template <> struct hash<ItemHandle> {
  auto operator()(ItemHandle const &s) const noexcept -> size_t;
};
}; // namespace std
