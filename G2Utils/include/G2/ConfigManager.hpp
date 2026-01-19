#pragma once

#include <filesystem>

#include "G2/Singleton.hpp"

struct Config final {
  bool show_console;
  std::filesystem::path base_dir = "g2utils/";

  auto log_path() const -> std::filesystem::path { return base_dir / "logs"; }
  auto dump_path() const -> std::filesystem::path { return base_dir / "dumps"; }
};

class ConfigManager final : public Singleton<ConfigManager> {
  friend class Singleton<ConfigManager>;

public:
  auto config() -> Config & { return m_config; }
  auto config() const -> const Config & { return m_config; }

  auto log_path() const -> std::filesystem::path { return config().log_path(); }
  auto dump_path() const -> std::filesystem::path {
    return config().dump_path();
  }

private:
  Config m_config;
};
