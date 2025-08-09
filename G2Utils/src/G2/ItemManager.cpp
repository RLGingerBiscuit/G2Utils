#include "G2/ItemManager.hpp"

SINGLETON_IMPL(ItemManager);

auto ItemManager::init_singleton() -> void {}

auto ItemManager::deinit_singleton() -> void {}

// auto ItemManager::get_item_info(ItemHandle handle) -> std::optional<ItemInfo> {
//   auto maybe_table = DataTableManager::instance().get_table_info(
//       DataTableHandle(handle.table_name()));
//   if (!maybe_table.has_value())
//     return {};

//   auto table = *maybe_table;
//   if (!table.items().contains(handle.item_name()))
//     return {};

//   // TODO: localised display name
//   return ItemInfo(handle.table_name(), handle.item_name(), handle.item_name());
// }
