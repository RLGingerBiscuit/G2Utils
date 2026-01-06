#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cassert>
#include <cstdlib>

#include "runme.hpp"

#define UNUSED(x) ((void)x);

static HMODULE g_module = nullptr;
static HANDLE g_thread = nullptr;
static bool g_running = false;

DWORD WINAPI ThreadEntry(LPVOID lpParam) {
  assert(g_running);
  assert(g_module != nullptr);
  assert(g_thread != nullptr);

  SetThreadDescription(g_thread, L"G2Utils Thread");

  runme();

  g_thread = nullptr;
  g_running = false;

  FreeLibraryAndExitThread(g_module, EXIT_SUCCESS);

  return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  switch (dwReason) {
  case DLL_PROCESS_ATTACH: {
    g_module = hModule;
    g_running = true;
    g_thread =
        CreateThread(nullptr, 0, ThreadEntry, (LPVOID)hModule, 0, nullptr);
  }; break;
  default:
    break;
  }

  return TRUE;
}
