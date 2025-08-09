#pragma once

#include <cstdint>
#include <optional>

#include "G2/Singleton.hpp"

struct PlayerHandle final {
  explicit PlayerHandle() {};
  explicit PlayerHandle(int32_t id, std::string name)
      : m_id(id), m_name(name) {};

  auto id() -> int32_t { return m_id; }
  auto id() const -> const int32_t { return m_id; }

  auto name() -> std::string & { return m_name; }
  auto name() const -> const std::string & { return m_name; }

private:
  int32_t m_id;
  std::string m_name;
};

class PlayerManager final : public Singleton<PlayerManager> {
  friend class Singleton<PlayerManager>;

public:
  auto get_player_by_id(int32_t target_id) -> std::optional<PlayerHandle>;
  auto get_player_by_name(std::string target_name)
      -> std::optional<PlayerHandle>;
  auto get_all_players() -> std::vector<PlayerHandle>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;
};
