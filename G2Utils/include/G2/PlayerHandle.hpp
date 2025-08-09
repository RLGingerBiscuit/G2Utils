#pragma once

#include <cstdint>
#include <string>

struct PlayerHandle final {
  explicit PlayerHandle() {};
  explicit PlayerHandle(int32_t id, std::string name)
      : m_id(id), m_name(name) {};

  auto id() -> int32_t { return m_id; }
  auto id() const -> const int32_t { return m_id; }

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

private:
  int32_t m_id;
  std::string m_name;
};
