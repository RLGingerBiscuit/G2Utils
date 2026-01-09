#pragma once

#include "G2/Singleton.hpp"
#include "G2/Theme.hpp"

class ThemeManager final : public Singleton<ThemeManager> {
  friend class Singleton<ThemeManager>;

protected:
  auto init_singleton() -> void override;

public:
  auto refresh() -> void;

  auto theme() -> Theme & { return m_theme; }
  auto theme() const -> const Theme & { return m_theme; }

private:
  Theme m_theme;
};
