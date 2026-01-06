#include "G2/DataTableManager.hpp"
#include "G2/ItemManager.hpp"

SINGLETON_IMPL(ItemManager);

auto ItemManager::init_singleton() -> void {}

auto ItemManager::deinit_singleton() -> void {}

auto ItemManager::is_valid_item(ItemHandle item) -> bool {
  // TODO: reduce copying
  auto table = DataTableManager::get().get_table_info(item.table_handle());
  if (!table.has_value())
    return false;

  return table->items().contains(item.item_name());
}
