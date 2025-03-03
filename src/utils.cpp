#include <SDL2/SDL.h>
#include <config.hpp>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unistd.h>
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

void restartApplication()
{
  char *basePath = SDL_GetBasePath();
  if (!basePath)
  {
    throw std::runtime_error("Couldn't get program path");
  }

  std::string programPath = std::string(basePath) + "minesweeper";
  SDL_free(basePath);

  char *argv[] = {programPath.data(), nullptr};
  execv(argv[0], argv);

  throw std::runtime_error("Failed to restart program");
}

bool isValidCell(const int row, const int col)
{
  const int maxRow = config::getSettings().getGridHeight();
  const int maxCol = config::getSettings().getGridWidth();
  return row >= 0 && col >= 0 && row < maxRow && col < maxCol;
};
} // namespace utils