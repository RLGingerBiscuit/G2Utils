#include "G2/Singleton.hpp"

#include <GLFW/glfw3.h>

class WindowManager : public Singleton<WindowManager> {
  friend class Singleton<WindowManager>;

public:
  auto begin_frame() -> void;
  auto end_frame() -> void;

  auto exit_requested() -> bool;
  auto request_exit() -> void;

  auto get_window_scale() -> float;

protected:
  auto init_singleton() -> void override;
  auto deinit_singleton() -> void override;

private:
  GLFWwindow *m_window{};
};
