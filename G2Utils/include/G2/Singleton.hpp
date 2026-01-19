#pragma once

#include <cassert>
#include <memory>

#include <spdlog/spdlog.h>

#define MAKE_NONMOVEABLE(T)                                                    \
  T(const T &&) = delete;                                                      \
  T &operator=(T &&) = delete;

#define MAKE_NONCOPYABLE(T)                                                    \
  T(const T &) = delete;                                                       \
  T &operator=(T &) = delete;

#define SINGLETON_IMPL(T)                                                      \
  template <> std::unique_ptr<T> Singleton<T>::m_instance = nullptr;

template <class T> class Singleton {
protected:
  Singleton() = default;
  ~Singleton() = default;

private:
  MAKE_NONCOPYABLE(Singleton);
  MAKE_NONMOVEABLE(Singleton);

protected:
  virtual auto init_singleton() -> void {}
  virtual auto deinit_singleton() -> void {}

public:
  static auto init() -> void {
    assert(m_instance == nullptr);
    m_instance = std::make_unique<T>();
    m_instance.get()->init_singleton();
    spdlog::debug("Initialised '{}'", typeid(T).name());
  }

  static auto deinit() -> void {
    assert(m_instance != nullptr);
    spdlog::debug("Deinitialising '{}'", typeid(T).name());
    m_instance.get()->deinit_singleton();
    m_instance = nullptr;
  }

  constexpr static auto get() -> T & {
    assert(m_instance != nullptr);
    return *m_instance;
  }

protected:
  static std::unique_ptr<T> m_instance;
};
