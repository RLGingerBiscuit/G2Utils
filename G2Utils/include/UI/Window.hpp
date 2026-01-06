#pragma once

#include <GLFW/glfw3.h>

#include "G2/Singleton.hpp"

class Window final {
private:
  MAKE_NONCOPYABLE(Window);
  MAKE_NONMOVEABLE(Window);

public:
  Window();
  ~Window();

  auto begin_frame() -> void;
  auto end_frame() -> void;

  auto exit_requested() -> bool;
  auto request_exit() -> void;

  auto get_window_scale() -> float;

private:
  GLFWwindow *m_window{};
};
