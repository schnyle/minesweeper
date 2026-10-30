#include "gui/input.hpp"

#include <SDL3/SDL_events.h>
#include <cmath>
#include <optional>

namespace input
{
namespace
{
[[nodiscard]] std::optional<ActionVariant>
_handle_mouse_down(const SDL_Event &event, const Layout &layout, const SDL_Point &point)
{
  switch (event.button.button)
  {
  case SDL_BUTTON_LEFT:
  {
    if (auto cell_pos = layout.cell_at(point))
    {
      return action::LeftClick{*cell_pos};
    }
    break;
  }
  case SDL_BUTTON_RIGHT:
  {
    if (auto cell_pos = layout.cell_at(point))
    {
      return action::RightClick{*cell_pos};
    }
    break;
  }
  case SDL_BUTTON_MIDDLE:
  {
    if (auto cell_pos = layout.cell_at(point))
    {
      return action::ChordClick{*cell_pos};
    }
    break;
  }
  default:
    return std::nullopt;
  }

  // unreachable
  return std::nullopt;
};
} // namespace

std::optional<ActionVariant> to_action(const SDL_Event &event, const Layout &layout)
{
  // SDL3 mouse coordinates are float; layout is in whole pixels.
  // Pixel i covers [i, i+1), so std::floor gives the pixel containing the mouse position.
  const SDL_Point point{
      .x = static_cast<int>(std::floor(event.motion.x)), .y = static_cast<int>(std::floor(event.motion.y))};

  switch (event.type)
  {
  case SDL_EVENT_QUIT:
  {
    return action::Quit{};
    break;
  }
  case SDL_EVENT_KEY_DOWN:
  {
    const SDL_Keycode keycode = event.key.key;
    if (keycode == SDLK_Q || keycode == SDLK_ESCAPE)
    {
      return action::Quit{};
    }
    break;
  }
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
  {
    if (auto res = _handle_mouse_down(event, layout, point))
    {
      return res;
    };
    break;
  }
  default:
    return std::nullopt;
  }
  return std::nullopt;
}
} // namespace input
