#include "app/app.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

static constexpr WindowSizePixels WINDOW_SIZE{.width = 1280, .height = 960};
static constexpr GridSize GRID_SIZE{.rows = 19, .cols = 24};
static constexpr LayoutStyle LAYOUT_STYLE{
    .divider_width = 20,
    .header_height_pct = 10.0,
    .reset_button_cell_span = 1.5,
};

int main()
{
  App app(WINDOW_SIZE, GRID_SIZE, LAYOUT_STYLE);
  app.run();
}
