#pragma once

#include "G2/Singleton.hpp"

class ConsoleManager final : public Singleton<ConsoleManager> {
  friend class Singleton<ConsoleManager>;

protected:
  auto init_singleton() -> void override;

private:
  void *s_console;
};
