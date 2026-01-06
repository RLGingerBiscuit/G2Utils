#pragma once

#include <cstdint>
#include <spdlog/fmt/fmt.h>
#include <string>

struct PlayerHandle final {
  explicit PlayerHandle() {};
  explicit PlayerHandle(int32_t id, std::string name)
      : m_id(id), m_name(name) {};

  auto id() -> int32_t { return m_id; }
  auto id() const -> const int32_t { return m_id; }

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

  auto operator==(const PlayerHandle &x) -> bool { return m_id == x.m_id; }
  auto operator==(const PlayerHandle &x) const -> bool {
    return m_id == x.m_id;
  }

  auto operator!=(const PlayerHandle &x) -> bool { return !(*this == x); }
  auto operator!=(const PlayerHandle &x) const -> bool { return !(*this == x); }

private:
  int32_t m_id;
  std::string m_name;
};

template <>
struct fmt::formatter<PlayerHandle> : fmt::formatter<fmt::string_view> {
  auto format(PlayerHandle handle, fmt::format_context &ctx) const
      -> fmt::format_context::iterator;
};

namespace std {
template <> struct hash<PlayerHandle> {
  auto operator()(PlayerHandle const &s) const noexcept -> size_t;
};
}; // namespace std
