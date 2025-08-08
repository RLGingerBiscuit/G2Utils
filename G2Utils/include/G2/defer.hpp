#pragma once

#include <utility>

template <class F> struct __privDefer {
  F f;
  __privDefer(F &&f) : f(std::forward<F>(f)) {}
  ~__privDefer() { f(); }
};
template <class F> __privDefer<F> __defer_func(F &&f) {
  return __privDefer<F>(std::forward<F>(f));
}

#define __DEFER_1(x, y) x##y
#define __DEFER_2(x, y) __DEFER_1(x, y)
#define __DEFER_3(x) __DEFER_2(x, __COUNTER__)
#define defer(code)                                                            \
  auto __DEFER_3(_defer_) = __defer_func([&]() -> void { code; })
