#pragma once

#include <functional>
#include <string>

#include <spdlog/fmt/fmt.h>

#include "G2/DataTableHandle.hpp"

struct ItemHandle final {
  explicit ItemHandle() {}
  explicit ItemHandle(DataTableHandle table, std::string name)
      : m_table(table), m_name(name) {}

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

  auto table() -> DataTableHandle & { return m_table; }
  auto table() const -> const DataTableHandle & { return m_table; }

  auto operator==(const ItemHandle &x) -> bool {
    return m_table == x.m_table && m_name == x.m_name;
  }
  auto operator==(const ItemHandle &x) const -> bool {
    return m_table == x.m_table && m_name == x.m_name;
  }

  auto operator!=(const ItemHandle &x) -> bool { return !(*this == x); }
  auto operator!=(const ItemHandle &x) const -> bool { return !(*this == x); }

private:
  DataTableHandle m_table;
  std::string m_name;
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
