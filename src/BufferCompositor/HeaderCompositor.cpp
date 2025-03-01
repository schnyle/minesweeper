#include <HeaderCompositor.hpp>
#include <algorithm>
#include <config.hpp>

#include "Rect.h"

void HeaderCompositor::buffInsertHeader(std::vector<uint32_t> &buff, const int width, const int buffSize)
{
  // base
  std::fill_n(buff.begin(), buffSize, config::GREY);

  // border
  BufferCompositor::buffInsert3DBorder(
      buff,
      width,
      {0, 0, config::GAME_WINDOW_PIXEL_WIDTH, config::GAME_WINDOW_PIXEL_HEIGHT},
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);

  BufferCompositor::buffInsert3DBorder(
      buff,
      width,
      {config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
       config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
       config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
       config::GAME_WINDOW_PIXEL_HEIGHT - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D)},
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);

  // header / game separator
  BufferCompositor::buffInsert3DBorder(
      buff,
      width,
      {config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
       config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
       config::FRAME_WIDTH},
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  // fill overlap
  BufferCompositor::buffInsertRectangle(
      buff,
      width,
      {config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + config::CELL_BORDER_WIDTH_3D,
       config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
       config::FRAME_WIDTH - 2 * config::CELL_BORDER_WIDTH_3D},
      config::GREY);
  // top-left corner
  BufferCompositor::buffInsert3DCorner(
      buff,
      width,
      {config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
       config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D},
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
  // top-right corner
  BufferCompositor::buffInsertRectangle(
      buff,
      width,
      {config::GAME_WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
       config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D},
      config::LIGHT_GREY);
  // bottom-left corner
  BufferCompositor::buffInsertRectangle(
      buff,
      width,
      {config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
       config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D},
      config::DARK_GREY);
  // bottom-right corner
  BufferCompositor::buffInsert3DCorner(
      buff,
      width,
      {config::GAME_WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
       config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D},
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
};

void HeaderCompositor::buffInsertRemainingFlags(
    std::vector<uint32_t> &buff,
    const int width,
    const Rect rect,
    const int n)
{
  BufferCompositor::buffInsertRectangle(buff, width, rect, config::BLACK);

  const int leftDigit = n / 100;
  const int middleDigit = (n % 100) / 10;
  const int rightDigit = (n % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::REMAINING_FLAGS_PAD;
  while ((rect.w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (rect.w - 4 * pad) / 3;
  const int digitHeight = rect.h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = rect.x + pad + i * (digitWidth + pad);
    const int digitY = rect.y + pad;

    buffInsertDigit(buff, width, {digitX, digitY, digitWidth, digitHeight}, digits[i], config::RED);
  }
};

void HeaderCompositor::buffInsertTimer(std::vector<uint32_t> &buff, const int width, const Rect rect, const int t)
{
  BufferCompositor::buffInsertRectangle(buff, width, rect, config::BLACK);

  const int leftDigit = t / 100;
  const int middleDigit = (t % 100) / 10;
  const int rightDigit = (t % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::REMAINING_FLAGS_PAD;
  while ((rect.w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (rect.w - 4 * pad) / 3;
  const int digitHeight = rect.h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = rect.x + pad + i * (digitWidth + pad);
    const int digitY = rect.y + pad;

    buffInsertDigit(buff, width, {digitX, digitY, digitWidth, digitHeight}, digits[i], config::RED);
  }
}

void HeaderCompositor::buffInsertGear(std::vector<uint32_t> &buff, const int width, const double center)
{
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double radius = size / 2 * 0.2;
  const double radiusSqrd = radius * radius;
  const double outerRadius = size / 2 * 0.5;
  const double outerRadiusSqrd = outerRadius * outerRadius;
  const double teethOuterRadius = size / 2 * 0.625;
  const double teethOuterRadiusSqrd = teethOuterRadius * teethOuterRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - center;
      const double dy = y - center;

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool innerCircle = dx * dx + dy * dy <= radiusSqrd;
      if (innerCircle)
      {
        buff[y * size + x] = config::GREY;
        continue;
      }

      const bool outerCircle = dx * dx + dy * dy <= outerRadiusSqrd;
      if (outerCircle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }

  const int numTeeth = 8;
  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      const double dx = x - center;
      const double dy = y - center;
      const double distSqrd = dx * dx + dy * dy;

      if (distSqrd < outerRadiusSqrd || distSqrd > teethOuterRadiusSqrd)
      {
        continue;
      }

      double theta = std::atan2(dy, dx);
      if (theta < 0)
      {
        theta += 2 * M_PI;
      }

      const int n = theta * (numTeeth * 2) / (2 * M_PI);
      if (n % 2 != 0)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
}