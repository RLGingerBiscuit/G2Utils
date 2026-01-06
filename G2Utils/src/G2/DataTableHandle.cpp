#include <spdlog/fmt/fmt.h>

#include "G2/DataTableHandle.hpp"

auto fmt::formatter<DataTableHandle>::format(DataTableHandle handle,
                                             fmt::format_context &ctx) const
    -> fmt::format_context::iterator {
  return format_to(ctx.out(), "{}", handle.name());
}

namespace std {
auto hash<DataTableHandle>::operator()(DataTableHandle const &s) const noexcept
    -> size_t {
  std::hash<std::string> hasher;
  return hasher(s.name());
}
}; // namespace std
