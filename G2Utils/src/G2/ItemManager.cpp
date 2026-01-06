#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"

SINGLETON_IMPL(ItemManager);

auto ItemManager::init_singleton() -> void {}

auto ItemManager::deinit_singleton() -> void {}

// auto ItemManager::get_item_info(ItemHandle handle) -> std::optional<ItemInfo>
// {
//   auto maybe_table = DataTableManager::get().get_table_info(
//       DataTableHandle(handle.table_name()));
//   if (!maybe_table.has_value())
//     return {};

//   auto table = *maybe_table;
//   if (!table.items().contains(handle.item_name()))
//     return {};

//   // TODO: localised display name
//   return ItemInfo(handle.table_name(), handle.item_name(),
//   handle.item_name());
// }

auto ItemManager::is_valid_item(ItemHandle item) -> bool {
  // TODO: reduce copying
  auto table = DataTableManager::get().get_table_info(item.table_handle());
  if (!table.has_value())
    return false;

  return table->items().contains(item.item_name());
}
