#include "gui/gui.hpp"
#include "gui/input.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <variant>

static int constexpr WINDOW_WIDTH_PIXELS = 1280;
static int constexpr WINDOW_HEIGHT_PIXEL = 960;
static int constexpr CELL_ROWS_COUNT = 19;
static int constexpr CELL_COLS_COUNT = 24;

class App
{
public:
  App(int window_width, int window_height, int cell_rows_count, int cell_cols_count)
      : engine_{cell_rows_count, cell_cols_count}, gui_{window_width, window_height, engine_}
  {
  }

  void run();

private:
  Engine engine_;
  Gui gui_;

  bool running_ = true;

  void handle_action(const input::ActionVariant &action);
};

void App::run()
{
  while (running_)
  {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (auto action = input::to_action(event, gui_.layout()))
    {
      handle_action(*action);
    }

    gui_.render(engine_);
  }
}

void App::handle_action(const input::ActionVariant &action)
{
  std::visit(
      [&](auto &&arg)
      {
        using namespace input::action;
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Quit>)
        {
          running_ = false;
        }
        else if constexpr (std::is_same_v<T, LeftClick>)
        {
          engine_.reveal(arg.pos.row, arg.pos.col);
        }
        else if constexpr (std::is_same_v<T, RightClick>)
        {
          engine_.flag(arg.pos.row, arg.pos.col);
        }
        else if constexpr (std::is_same_v<T, ChordClick>)
        {
          engine_.chord(arg.pos.row, arg.pos.col);
        }
        else
        {
          static_assert(false, "unhandled action");
        }
      },
      action);
}

int main()
{
  App app(WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXEL, CELL_ROWS_COUNT, CELL_COLS_COUNT);
  app.run();
}
