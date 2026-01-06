#include "G2/ItemHandle.hpp"

#include <spdlog/fmt/fmt.h>

#include "G2/DataTableHandle.hpp"

auto fmt::formatter<ItemHandle>::format(ItemHandle handle,
                                        fmt::format_context &ctx) const
    -> fmt::format_context::iterator {
  return format_to(ctx.out(), "{}.{}", handle.table_handle(),
                   handle.item_name());
}

namespace std {
auto hash<ItemHandle>::operator()(ItemHandle const &s) const noexcept
    -> std::size_t {
  std::hash<std::string> h1;
  std::hash<DataTableHandle> h2;
  size_t seed = h1(s.item_name());
  seed ^= h2(s.table_handle()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}
}; // namespace std
