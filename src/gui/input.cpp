#include "gui/input.hpp"
#include "engine/engine.hpp"
#include "gui/layout.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <cmath>
#include <optional>
#include <utility>

namespace input
{
namespace
{
// SDL3 mouse coordinates are float; layout is in whole pixels.
// Pixel i covers [i, i+1), so std::floor gives the pixel containing the mouse position.
[[nodiscard]] SDL_Point _to_point(const SDL_MouseButtonEvent &button)
{
  return {.x = static_cast<int>(std::floor(button.x)), .y = static_cast<int>(std::floor(button.y))};
}

[[nodiscard]] std::optional<Action>
_handle_mouse_down(const SDL_Event &event, const Layout &layout, std::optional<Press> &pressed)
{
  pressed.reset();

  const auto target = layout.hit_test(_to_point(event.button));
  if (!target)
  {
    return std::nullopt;
  }

  switch (event.button.button)
  {
  case SDL_BUTTON_RIGHT:
  {
    if (const auto *cell = std::get_if<CellPosition>(&*target))
    {
      return action::Flag{*cell};
    }
    break;
  }
  case SDL_BUTTON_LEFT:
  case SDL_BUTTON_MIDDLE:
  {
    pressed = Press{*target, event.button.button};
    return std::nullopt;
  }
  default:
    break;
  }

  return std::nullopt;
};

[[nodiscard]] std::optional<Action>
_handle_mouse_button_up(const SDL_Event &event, const Layout &layout, std::optional<Press> &pressed)
{
  const auto press = std::exchange(pressed, std::nullopt);
  if (!press || press->button != event.button.button)
  {
    return std::nullopt;
  }

  const auto target = layout.hit_test(_to_point(event.button));
  if (!target || *target != press->target)
  {
    return std::nullopt;
  }

  const auto *cell = std::get_if<CellPosition>(&*target);

  switch (event.button.button)
  {
  case SDL_BUTTON_LEFT:
  {
    if (std::holds_alternative<ResetButton>(*target))
    {
      return action::Reset{};
    }
    if (cell != nullptr)
    {
      return action::Reveal{*cell};
    }
    break;
  }
  case SDL_BUTTON_MIDDLE:
  {
    if (cell != nullptr)
    {
      return action::Chord{*cell};
    }
    break;
  }
  default:
    break;
  }

  return std::nullopt;
}
} // namespace

std::optional<Action> to_action(const SDL_Event &event, const Layout &layout, std::optional<Press> &pressed)
{
  switch (event.type)
  {
  case SDL_EVENT_QUIT:
  {
    return action::Quit{};
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
    return _handle_mouse_down(event, layout, pressed);
  }
  case SDL_EVENT_MOUSE_BUTTON_UP:
  {
    return _handle_mouse_button_up(event, layout, pressed);
  }
  default:
    break;
  }
  return std::nullopt;
}
} // namespace input
