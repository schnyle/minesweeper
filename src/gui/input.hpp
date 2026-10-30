#pragma once

#include <SDL3/SDL_events.h>
#include <optional>
#include <variant>

#include "engine/engine.hpp"
#include "gui/layout.hpp"

namespace input
{

struct Press
{
  Press(HitTarget target, Uint8 button) : target(target), button(button) {}

  HitTarget target;
  Uint8 button; // e.g. SDL_BUTTON_LEFT
};

namespace action
{
struct Reveal
{
  CellPosition pos;
};

struct Flag
{
  CellPosition pos;
};

struct Chord
{
  CellPosition pos;
};

struct Reset
{
};

struct Quit
{
};
} // namespace action

using Action = std::variant<action::Reveal, action::Flag, action::Chord, action::Reset, action::Quit>;

std::optional<Action> to_action(const SDL_Event &event, const Layout &layout, std::optional<Press> &pressed);

} // namespace input
