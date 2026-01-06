#include <spdlog/fmt/fmt.h>

#include "G2/ItemHandle.hpp"

auto fmt::formatter<ItemHandle>::format(ItemHandle handle,
                                        fmt::format_context &ctx) const
    -> fmt::format_context::iterator {
  return format_to(ctx.out(), "{}.{}", handle.table_handle(),
                   handle.item_name());
}
