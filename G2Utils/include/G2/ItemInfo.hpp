#pragma once

#include <string>

#include "G2/ItemHandle.hpp"

struct ItemInfo final {
  explicit ItemInfo() {}
  explicit ItemInfo(ItemHandle handle, std::string display_name)
      : m_handle(handle), m_display_name(display_name) {}

  auto handle() -> ItemHandle & { return m_handle; }
  auto handle() const -> const ItemHandle & { return m_handle; }

  auto name() -> std::string & { return handle().name(); }
  auto name() const -> const std::string & { return handle().name(); }

  auto display_name() -> std::string & { return m_display_name; }
  auto display_name() const -> const std::string & { return m_display_name; }

private:
  ItemHandle m_handle;
  std::string m_display_name;
};
