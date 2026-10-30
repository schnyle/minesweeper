#include <SDL3/SDL_events.h>
#include <chrono>

#include "app/app.hpp"
#include "gui/layout.hpp"

App::App(WindowSizePixels window_size, GridSize grid_size, LayoutStyle layout_style)
    : engine_{grid_size.rows, grid_size.cols}, gui_{window_size, layout_style, engine_}
{
}

void App::run()
{
  while (is_running_)
  {
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, until_next_timer_tick()))
    {
      if (auto action = gui_.handle(event))
      {
        handle_action(*action);
      }
    };

    sync_timer();
    const auto elapsed_seconds = timer_.elapsed().transform(
        [](auto elapsed) { return static_cast<int>(std::chrono::floor<std::chrono::seconds>(elapsed).count()); });
    gui_.render(engine_, elapsed_seconds);
  }
}

void App::handle_action(const input::Action &action)
{
  std::visit(
      [&](auto &&arg)
      {
        using namespace input::action;
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Chord>)
        {
          engine_.chord(arg.pos);
        }
        else if constexpr (std::is_same_v<T, Flag>)
        {
          engine_.flag(arg.pos);
        }
        else if constexpr (std::is_same_v<T, Reveal>)
        {
          engine_.reveal(arg.pos);
        }
        else if constexpr (std::is_same_v<T, Reset>)
        {
          timer_.reset();
          engine_.reset();
        }
        else if constexpr (std::is_same_v<T, Quit>)
        {
          is_running_ = false;
        }
        else
        {
          static_assert(false, "unhandled action");
        }
      },
      action);
}

void App::sync_timer()
{
  using enum Engine::Status;
  switch (engine_.status())
  {
  case NotStarted:
    timer_.reset();
    break;
  case Playing:
    if (!timer_.started())
    {
      timer_.start();
    }
    break;
  case Won:
  case Lost:
    if (timer_.running())
    {
      timer_.stop();
    }
    break;
  }
}

Sint32 App::until_next_timer_tick() const
{
  using namespace std::chrono_literals;

  auto t = IDLE_WAKE;
  const auto elapsed = timer_.elapsed();
  if (timer_.running() && elapsed)
  {
    t = std::chrono::ceil<std::chrono::milliseconds>(1s - (*elapsed % 1s));
  }

  return static_cast<Sint32>(t.count());
}
