#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

namespace utils
{
SDL_Color hexToRgba(uint32_t hexColor);

bool isPointInRect(const int x, const int y, const SDL_Rect &rect);

void restartApplication();
}