#include "G2/StringManager.hpp"

#include <memory>
#include <span>
#include <string>
#include <unordered_map>

#include <SDK/SDK/Basic.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/UnrealContainers.hpp>
#include <SDK/UtfN.hpp>

SINGLETON_IMPL(StringManager);

template <class T> static size_t fnv1a(const std::span<const T> span) {
  constexpr size_t offset = 14695981039346656037ull;
  constexpr size_t prime = 1099511628211ull;

  std::span<const uint8_t> bytes{reinterpret_cast<const uint8_t *>(span.data()),
                                 span.size_bytes()};
  size_t hash = offset;
  for (size_t i = 0; i < bytes.size(); ++i) {
    hash ^= static_cast<size_t>(bytes[i]);
    hash *= prime;
  }

  return hash;
}

namespace std {
template <> struct hash<SDK::FString> {
  std::size_t operator()(SDK::FString const &s) const noexcept {
    return fnv1a(std::span(s.GetDataPtr(), s.Num()));
  }
};
}; // namespace std

namespace std {
template <> struct hash<SDK::FName> {
  std::size_t operator()(SDK::FName const &s) const noexcept {
    return static_cast<size_t>(s.ComparisonIndex) << 32 |
           static_cast<size_t>(s.Number);
  }
};
}; // namespace std

class StringCache final {
private:
  struct EntryStruct {
    std::string str;
    std::wstring wstr;
    SDK::FString fstring;
    SDK::FName fname;
  };

public:
  using EntryType = std::shared_ptr<EntryStruct>;

private:
  auto make_entry(const std::string &str, const std::wstring &wstr,
                  const SDK::FString &fstring, const SDK::FName &fname)
      -> EntryType {
    return std::make_shared<EntryStruct>(str, wstr, fstring, fname);
  }

  auto make_entry(const std::string &str) -> EntryType {
    auto wstr = UtfN::Utf8StringToUtf16String<std::wstring>(str);
    auto fstring = SDK::FString(wstr.c_str());
    auto fname = SDK::UKismetStringLibrary::Conv_StringToName(fstring);
    return make_entry(str, wstr, fstring, fname);
  }

  auto make_entry(const std::wstring &wstr) -> EntryType {
    auto str = UtfN::Utf16StringToUtf8String<std::string>(wstr);
    auto fstring = SDK::FString(wstr.c_str());
    auto fname = SDK::UKismetStringLibrary::Conv_StringToName(fstring);
    return make_entry(str, wstr, fstring, fname);
  }

  auto make_entry(const SDK::FString &fstring) -> EntryType {
    auto str = fstring.ToString();
    auto wstr = fstring.ToWString();
    auto fname = SDK::UKismetStringLibrary::Conv_StringToName(fstring);
    return make_entry(str, wstr, fstring, fname);
  }

  auto make_entry(const SDK::FName &fname) -> EntryType {
    auto str = fname.ToString();
    auto wstr = UtfN::Utf8StringToUtf16String<std::wstring>(str);
    auto fstring = SDK::FString(wstr.c_str());
    return make_entry(str, wstr, fstring, fname);
  }

  auto add(const std::string &str, const std::wstring &wstr,
           const SDK::FString &fstring, const SDK::FName &fname) -> EntryType {
    auto entry = make_entry(str, wstr, fstring, fname);
    m_string[str] = entry;
    m_wstring[wstr] = entry;
    m_fstring[fstring] = entry;
    m_fname[fname] = entry;
    return entry;
  }

public:
  auto get_or_add(const std::string &str) -> EntryType {
    return m_string.try_emplace(str, make_entry(str)).first->second;
  }
  auto get_or_add(const std::wstring &wstr) -> EntryType {
    return m_wstring.try_emplace(wstr, make_entry(wstr)).first->second;
  }
  auto get_or_add(const SDK::FString &fstring) -> EntryType {
    return m_fstring.try_emplace(fstring, make_entry(fstring)).first->second;
  }
  auto get_or_add(const SDK::FName &fname) -> EntryType {
    return m_fname.try_emplace(fname, make_entry(fname)).first->second;
  }

private:
  template <class TKey>
  using CacheMap = std::unordered_map<TKey, std::shared_ptr<EntryStruct>>;

  CacheMap<std::string> m_string;
  CacheMap<std::wstring> m_wstring;
  CacheMap<SDK::FString> m_fstring;
  CacheMap<SDK::FName> m_fname;
};

static StringCache s_cache;

auto StringManager::init_singleton() -> void {}

auto StringManager::deinit_singleton() -> void {}

auto StringManager::to_string(const std::wstring &wstr) -> std::string {
  return s_cache.get_or_add(wstr)->str;
}
auto StringManager::to_string(const SDK::FString &fstring) -> std::string {
  return s_cache.get_or_add(fstring)->str;
}
auto StringManager::to_string(const SDK::FName &fname) -> std::string {
  return s_cache.get_or_add(fname)->str;
}

auto StringManager::to_wstring(const std::string &str) -> std::wstring {
  return s_cache.get_or_add(str)->wstr;
}
auto StringManager::to_wstring(const SDK::FString &fstring) -> std::wstring {
  return s_cache.get_or_add(fstring)->wstr;
}
auto StringManager::to_wstring(const SDK::FName &fname) -> std::wstring {
  return s_cache.get_or_add(fname)->wstr;
}

auto StringManager::to_fstring(const std::string &str) -> SDK::FString {
  return s_cache.get_or_add(str)->fstring;
}
auto StringManager::to_fstring(const std::wstring &wstr) -> SDK::FString {
  return s_cache.get_or_add(wstr)->fstring;
}
auto StringManager::to_fname(const SDK::FString &fstring) -> SDK::FName {
  return s_cache.get_or_add(fstring)->fname;
}

auto StringManager::to_fname(const std::string &str) -> SDK::FName {
  return s_cache.get_or_add(str)->fname;
}
auto StringManager::to_fname(const std::wstring &wstr) -> SDK::FName {
  return s_cache.get_or_add(wstr)->fname;
}
auto StringManager::to_fstring(const SDK::FName &fname) -> SDK::FString {
  return s_cache.get_or_add(fname)->fstring;
}

namespace String {

auto to_string(const std::wstring &wstr) -> std::string {
  return StringManager::get().to_string(wstr);
}
auto to_string(const SDK::FString &fstring) -> std::string {
  return StringManager::get().to_string(fstring);
}
auto to_string(const SDK::FName &fname) -> std::string {
  return StringManager::get().to_string(fname);
}

auto to_wstring(const std::string &str) -> std::wstring {
  return StringManager::get().to_wstring(str);
}
auto to_wstring(const SDK::FString &fstring) -> std::wstring {
  return StringManager::get().to_wstring(fstring);
}
auto to_wstring(const SDK::FName &fname) -> std::wstring {
  return StringManager::get().to_wstring(fname);
}

auto to_fstring(const std::string &str) -> SDK::FString {
  return StringManager::get().to_fstring(str);
}

auto to_fstring(const std::wstring &wstr) -> SDK::FString {
  return StringManager::get().to_fstring(wstr);
}

auto to_fstring(const SDK::FName &fname) -> SDK::FString {
  return StringManager::get().to_fstring(fname);
}

auto to_fname(const std::string &str) -> SDK::FName {
  return StringManager::get().to_fname(str);
}
auto to_fname(const std::wstring &wstr) -> SDK::FName {
  return StringManager::get().to_fname(wstr);
}
auto to_fname(const SDK::FString &fstring) -> SDK::FName {
  return StringManager::get().to_fname(fstring);
}

}; // namespace String
