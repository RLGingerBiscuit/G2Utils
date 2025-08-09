#pragma once

#include <string>

struct DataTableHandle final {
  explicit DataTableHandle() {}
  explicit DataTableHandle(std::string name) : m_name(name) {}

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

private:
  std::string m_name;
};
