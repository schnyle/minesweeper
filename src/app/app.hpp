#pragma once

#include "app/timer.hpp"
#include "engine/engine.hpp"
#include "gui/gui.hpp"
#include "gui/input.hpp"
#include "gui/layout.hpp"

class App
{
public:
  App(WindowSizePixels window_size, GridSize grid_size, LayoutStyle layout_style);

  void run();

private:
  static constexpr std::chrono::milliseconds IDLE_WAKE{1000};

  Engine engine_;
  Gui gui_;
  Timer timer_;

  bool is_running_ = true;

  void handle_action(const input::Action &action);

  void sync_timer();

  [[nodiscard]] Sint32 until_next_timer_tick() const;
};
