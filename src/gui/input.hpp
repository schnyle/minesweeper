#pragma once

#include <SDL3/SDL_events.h>
#include <optional>
#include <variant>

#include "gui/layout.hpp"

namespace input
{
namespace action
{
struct Quit
{
};

struct LeftClick
{
  CellPosition pos;
};

struct RightClick
{
  CellPosition pos;
};

struct ChordClick
{
  CellPosition pos;
};

} // namespace action

using ActionVariant = std::variant<action::LeftClick, action::RightClick, action::ChordClick, action::Quit>;

std::optional<ActionVariant> to_action(const SDL_Event &event, const Layout &layout);

} // namespace input
