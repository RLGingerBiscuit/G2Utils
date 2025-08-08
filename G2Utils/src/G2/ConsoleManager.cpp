#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdio>

#include <spdlog/spdlog.h>

#include "G2/ConsoleManager.hpp"

SINGLETON_IMPL(ConsoleManager);

#define CONSOLE_WINDOW() static_cast<HWND>(this->s_console)

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
  switch (dwCtrlType) {
  case CTRL_C_EVENT:
    // Ignore Ctrl-C
    return TRUE;

  case CTRL_CLOSE_EVENT:
    // 'Closed' isn't really closed for us...
    ConsoleManager::instance().hide();
    return TRUE;

  default:
    return FALSE;
  }
}

auto ConsoleManager::init_singleton() -> void {
  AllocConsole();
  std::freopen("CONOUT$", "w", stdout);
  std::freopen("CONOUT$", "w", stderr);
  std::freopen("CONIN$", "r", stdin);
  this->s_console = GetConsoleWindow();

  // Override close button
  SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

  hide();
}

auto ConsoleManager::deinit_singleton() -> void {
  FreeConsole();
  if (this->s_console == GetConsoleWindow() && IsWindow(CONSOLE_WINDOW())) {
    PostMessage(CONSOLE_WINDOW(), WM_CLOSE, 0, 0);
    while (this->s_console == GetConsoleWindow())
      Sleep(100);
  }
  this->s_console = nullptr;
}

auto ConsoleManager::show() -> void {
  assert(this->s_console != nullptr);
  ShowWindow(CONSOLE_WINDOW(), SW_SHOW);
}

auto ConsoleManager::hide() -> void {
  assert(this->s_console != nullptr);
  ShowWindow(CONSOLE_WINDOW(), SW_HIDE);
}
