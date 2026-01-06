#pragma once

#include "G2/Singleton.hpp"

class ConsoleManager final : public Singleton<ConsoleManager> {
  friend class Singleton<ConsoleManager>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;

public:
  auto show() -> void;
  auto hide() -> void;

private:
  void *m_console;
};
