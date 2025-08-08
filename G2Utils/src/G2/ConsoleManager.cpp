#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdio>

#include "G2/ConsoleManager.hpp"

SINGLETON_IMPL(ConsoleManager);

auto ConsoleManager::init_singleton() -> void {
  AllocConsole();
  std::freopen("CONOUT$", "w", stdout);
  std::freopen("CONOUT$", "w", stderr);
  std::freopen("CONIN$", "r", stdin);
  this->s_console = GetConsoleWindow();
  ShowWindow(static_cast<HWND>(this->s_console), SW_SHOW);
}
