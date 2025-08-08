#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
// Windows be windowsing
#include <TlHelp32.h>

#include <cassert>
#include <chrono>
#include <filesystem>
#include <thread>
#include <unordered_map>

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/std.h>
#include <spdlog/spdlog.h>

#include "defer.hpp"

using namespace std::chrono_literals;

void wait_for_input() { char c = std::getchar(); }
#define wait_and_return(ret)                                                   \
  wait_for_input();                                                            \
  return ret;

enum class G2Variant {
  GRTS,
  GDK,
};
template <>
struct fmt::formatter<G2Variant> : fmt::formatter<fmt::string_view> {
  auto format(G2Variant variant, fmt::format_context &ctx) const
      -> fmt::format_context::iterator {
    fmt::string_view variant_str = "UNKNOWN";
    // clang-format off
    switch (variant) {
      case G2Variant::GRTS: variant_str = "GRTS"; break;
      case G2Variant::GDK: variant_str = "GDK"; break;
    }
    // clang-format on
    return fmt::formatter<fmt::string_view>::format(variant_str, ctx);
  }
};

DWORD get_g2_process(G2Variant *out_variant);

static const std::unordered_map<G2Variant, std::string> VARIANT_EXES = {
    {G2Variant::GRTS, std::string("Grounded2-WinGRTS-Shipping.exe")},
    {G2Variant::GDK, std::string("Grounded2-WinGDK-Shipping.exe")},
};

int main() {
  DWORD dwTargetProcessId = 0;
  G2Variant variant;
  while (true) {
    dwTargetProcessId = get_g2_process(&variant);
    if (dwTargetProcessId == 0) {
      spdlog::warn("Could not find Steam/Gamepass Grounded 2, waiting 5 "
                   "seconds then retrying (Press Ctrl-C to stop)...");
      std::this_thread::sleep_for(5s);
    } else {
      break;
    }
  }
  assert(dwTargetProcessId > 0);

  spdlog::info("Found Grounded 2 process: '{}' (Variant: {}, PID: {})",
               VARIANT_EXES.at(variant), variant, dwTargetProcessId);

  const auto cwd = std::filesystem::current_path();
  const auto dllName = fmt::format("G2Utils-{}.dll", variant);
  const auto dllPath = cwd / dllName;
  const auto dllPathStr = dllPath.string();

  if (!std::filesystem::exists(dllPath)) {
    spdlog::critical("{} does not exist, ensure it is next to the Injector!",
                  dllName);
    wait_and_return(1);
  }

  spdlog::info("DLL Path: {}", dllPath);

  HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ |
                                    PROCESS_VM_WRITE | PROCESS_CREATE_THREAD,
                                FALSE, dwTargetProcessId);
  if (hProcess == nullptr) {
    spdlog::critical("Could not open process for injection: {}", GetLastError());
    wait_and_return(1);
  }
  defer(CloseHandle(hProcess));

  spdlog::info("Opened handle: {:p}", hProcess);

  LPVOID lpDllNameMemory =
      VirtualAllocEx(hProcess, NULL, dllPathStr.size() + 1,
                     MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (lpDllNameMemory == nullptr) {
    spdlog::critical("Could not allocate memory in Grounded 2 process: {}",
                  GetLastError());
    wait_and_return(1);
  }

  spdlog::info("Allocated memory in Grounded 2 process: {:p}", lpDllNameMemory);

  SIZE_T bytesWritten = 0;
  if (!WriteProcessMemory(hProcess, lpDllNameMemory, dllPathStr.c_str(),
                          dllPathStr.size() + 1, &bytesWritten)) {
    spdlog::critical("Could not inject dll into Grounded 2 process: {}",
                  GetLastError());
    wait_and_return(1);
  }

  spdlog::info("Wrote {} bytes to Grounded 2 process memory", bytesWritten);

  HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
  if (hKernel32 == nullptr) {
    spdlog::critical("Could not get handle to kernel32.dll: {}", GetLastError());
    wait_and_return(1);
  }

  FARPROC lpLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");
  if (lpLoadLibraryA == nullptr) {
    spdlog::critical("Could not get handle to LoadLibraryA: {}", GetLastError());
    wait_and_return(1);
  }

  spdlog::info("LoadLibraryA Address: {:p}",
               reinterpret_cast<void *>(lpLoadLibraryA));

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
                                      (LPTHREAD_START_ROUTINE)lpLoadLibraryA,
                                      lpDllNameMemory, 0, NULL);
  if (hThread == nullptr) {
    spdlog::critical("Could not create remote thread in Grounded 2 process: {}",
                  GetLastError());
    wait_and_return(1);
  }
  defer(CloseHandle(hThread));

  spdlog::info("Created remote thread in Grounded 2 process: {:p}",hThread);

  WaitForSingleObject(hThread, INFINITE);
  spdlog::info("Success!");

  return 0;
}

DWORD get_g2_process(G2Variant *out_variant) {
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot == INVALID_HANDLE_VALUE) {
    spdlog::critical("Could not create snapshot: {}", GetLastError());
    wait_and_return(1);
  }
  defer(CloseHandle(hSnapshot));

  PROCESSENTRY32 procEntry32 = {
      .dwSize = sizeof(PROCESSENTRY32),
  };

  if (!Process32First(hSnapshot, &procEntry32)) {
    spdlog::critical("Could not retrieve process information: {}", GetLastError());
    return 0;
  }

  DWORD dwTargetProcessId = 0;
  do {
    const auto procExe = std::string(procEntry32.szExeFile);
    for (const auto x : VARIANT_EXES) {
      if (procExe == x.second) {
        dwTargetProcessId = procEntry32.th32ProcessID;
        if (out_variant)
          *out_variant = x.first;
        break;
      }
    }
  } while (Process32Next(hSnapshot, &procEntry32));

  return dwTargetProcessId;
}
