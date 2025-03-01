#include <SDL2/SDL.h>
#include <cstdint>
#include <utils.hpp>

namespace utils
{
SDL_Color hexToRgba(uint32_t hexColor)
{
  SDL_Color color;
  color.r = (hexColor >> 24) & 0xff;
  color.g = (hexColor >> 16) & 0xff;
  color.b = (hexColor >> 8) & 0xff;
  color.a = (hexColor >> 0) & 0xff;
  return color;
};

bool isPointInRect(const int x, const int y, const SDL_Rect &rect)
{
  return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
};
} // namespace utils