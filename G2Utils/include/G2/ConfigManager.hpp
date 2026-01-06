#pragma once

#include "G2/Singleton.hpp"

struct Config final {
  bool show_console;
};

class ConfigManager final : public Singleton<ConfigManager> {
  friend class Singleton<ConfigManager>;

public:
  auto config() -> Config & { return m_config; }
  auto config() const -> const Config & { return m_config; }

private:
  Config m_config;
};
