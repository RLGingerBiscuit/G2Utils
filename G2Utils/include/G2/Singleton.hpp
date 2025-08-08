#pragma once

#include <cassert>
#include <memory>

#define MAKE_NONMOVEABLE(T)                                                    \
  T(const T &&) = delete;                                                      \
  T &operator=(T &&) = delete;

#define MAKE_NONCOPYABLE(T)                                                    \
  T(const T &) = delete;                                                       \
  T &operator=(T &) = delete;

#define SINGLETON_IMPL(T)                                                      \
  template <> std::unique_ptr<T> Singleton<T>::m_instance = nullptr;

template <class T> class Singleton {
public:
  ~Singleton() = default;

protected:
  Singleton() = default;

  MAKE_NONCOPYABLE(Singleton);
  MAKE_NONMOVEABLE(Singleton);

protected:
  virtual auto init_singleton() -> void = 0;

public:
  static auto init() -> void {
    assert(m_instance == nullptr);
    m_instance = std::make_unique<T>();
    m_instance.get()->init_singleton();
  }

  constexpr static auto instance() -> T & {
    assert(m_instance != nullptr);
    return *m_instance;
  }

protected:
  static std::unique_ptr<T> m_instance;
};
