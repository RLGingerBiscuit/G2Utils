#pragma once

#include <spdlog/fmt/fmt.h>
#include <string>

struct DataTableHandle final {
  explicit DataTableHandle() {}
  explicit DataTableHandle(std::string name) : m_name(name) {}

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

  auto operator==(const DataTableHandle &x) -> bool {
    return m_name == x.m_name;
  }
  auto operator==(const DataTableHandle &x) const -> bool {
    return m_name == x.m_name;
  }

  auto operator!=(const DataTableHandle &x) -> bool { return !(*this == x); }
  auto operator!=(const DataTableHandle &x) const -> bool {
    return !(*this == x);
  }

private:
  std::string m_name;
};

template <>
struct fmt::formatter<DataTableHandle> : fmt::formatter<fmt::string_view> {
  auto format(DataTableHandle handle, fmt::format_context &ctx) const
      -> fmt::format_context::iterator;
};
