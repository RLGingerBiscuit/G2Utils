#include "G2/PlayerHandle.hpp"

#include <spdlog/fmt/fmt.h>

auto fmt::formatter<PlayerHandle>::format(PlayerHandle handle,
                                          fmt::format_context &ctx) const
    -> fmt::format_context::iterator {
  return format_to(ctx.out(), "'{}' ({})", handle.name(), handle.id());
}

namespace std {
auto hash<PlayerHandle>::operator()(PlayerHandle const &s) const noexcept
    -> size_t {
  return s.id();
}
}; // namespace std
