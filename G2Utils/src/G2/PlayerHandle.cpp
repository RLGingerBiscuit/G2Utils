#include <spdlog/fmt/fmt.h>

#include "G2/PlayerHandle.hpp"

auto fmt::formatter<PlayerHandle>::format(PlayerHandle handle,
                                          fmt::format_context &ctx) const
    -> fmt::format_context::iterator {
  return format_to(ctx.out(), "'{}' ({})", handle.name(), handle.id());
}
