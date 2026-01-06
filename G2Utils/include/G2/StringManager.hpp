#pragma once

#include <string>

#include "G2/Singleton.hpp"

// FIXME: Preferably, I want to keep all mentions of the SDK functions to the
//        .cpp files. This ain't that.
namespace UC {
class FString;
}; // namespace UC

namespace SDK {
using namespace UC;
class FName;
}; // namespace SDK

class StringManager final : public Singleton<StringManager> {
  friend class Singleton<StringManager>;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;

public:
  auto to_string(const std::wstring &wstr) -> std::string;
  auto to_string(const SDK::FString &fstring) -> std::string;
  auto to_string(const SDK::FName &fname) -> std::string;

  auto to_wstring(const std::string &str) -> std::wstring;
  auto to_wstring(const SDK::FString &fstring) -> std::wstring;
  auto to_wstring(const SDK::FName &fname) -> std::wstring;

  auto to_fstring(const std::string &str) -> SDK::FString;
  auto to_fstring(const std::wstring &wstr) -> SDK::FString;
  auto to_fstring(const SDK::FName &fname) -> SDK::FString;

  auto to_fname(const std::string &str) -> SDK::FName;
  auto to_fname(const std::wstring &wstr) -> SDK::FName;
  auto to_fname(const SDK::FString &fstring) -> SDK::FName;
};

namespace String {

auto to_string(const std::wstring &wstr) -> std::string;
auto to_string(const SDK::FString &fstring) -> std::string;
auto to_string(const SDK::FName &fname) -> std::string;

auto to_wstring(const std::string &str) -> std::wstring;
auto to_wstring(const SDK::FString &fstring) -> std::wstring;
auto to_wstring(const SDK::FName &fname) -> std::wstring;

auto to_fstring(const std::string &str) -> SDK::FString;
auto to_fstring(const std::wstring &wstr) -> SDK::FString;
auto to_fstring(const SDK::FName &fname) -> SDK::FString;

auto to_fname(const std::string &str) -> SDK::FName;
auto to_fname(const std::wstring &wstr) -> SDK::FName;
auto to_fname(const SDK::FString &fstring) -> SDK::FName;

}; // namespace String
