#include <BufferCompositor.hpp>
#include <algorithm>
#include <config.hpp>
#include <cstdint>
#include <stdexcept>

BufferCompositor::DigitSegments BufferCompositor::intToDigitSegments(const int n)
{
  if (n < 0 || n > 9)
  {
    throw std::out_of_range("Digit must be in [0, 9]");
  }

  switch (n)
  {
  case 0:
    return {true, true, true, false, true, true, true};
  case 1:
    return {false, false, true, false, false, false, true};
  case 2:
    return {true, false, true, true, true, true, false};
  case 3:
    return {true, false, true, true, true, false, true};
  case 4:
    return {false, true, true, true, false, false, true};
  case 5:
    return {true, true, false, true, true, false, true};
  case 6:
    return {true, true, false, true, true, true, true};
  case 7:
    return {true, false, true, false, false, false, true};
  case 8:
    return {true, true, true, true, true, true, true};
  case 9:
    return {true, true, true, true, false, false, true};
  }

  throw std::runtime_error("Error converting int to DigitSegments");
}

void BufferCompositor::buffInsertRectangle(
    std::vector<uint32_t> &buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t c)
{
  for (int row = y; row < y + h; ++row)
  {
    const int index = row * buffWidth + x;
    std::fill_n(buff.begin() + index, w, c);
  }
};

void BufferCompositor::buffInsert2DBorder(
    std::vector<uint32_t> &buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t c)
{
  // left edge
  buffInsertRectangle(buff, buffWidth, x, y, config::CELL_BORDER_WIDTH_2D, h, c);
  // top edge
  buffInsertRectangle(buff, buffWidth, x, y, w, config::CELL_BORDER_WIDTH_2D, c);
  // right edge
  buffInsertRectangle(buff, buffWidth, w - config::CELL_BORDER_WIDTH_2D, y, config::CELL_BORDER_WIDTH_2D, h, c);
  // left edge
  buffInsertRectangle(buff, buffWidth, x, h - config::CELL_BORDER_WIDTH_2D, w, config::CELL_BORDER_WIDTH_2D, c);
}

void BufferCompositor::buffInsert3DBorder(
    std::vector<uint32_t> &buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t cTop,
    const uint32_t cMid,
    const uint32_t cBot)
{
  // left edge
  buffInsertRectangle(buff, buffWidth, x, y, config::CELL_BORDER_WIDTH_3D, h, cTop);
  // top edge
  buffInsertRectangle(buff, buffWidth, x, y, w, config::CELL_BORDER_WIDTH_3D, cTop);
  // right edge
  buffInsertRectangle(buff, buffWidth, x + w - config::CELL_BORDER_WIDTH_3D, y, config::CELL_BORDER_WIDTH_3D, h, cBot);
  // bottom edge
  buffInsertRectangle(buff, buffWidth, x, y + h - config::CELL_BORDER_WIDTH_3D, w, config::CELL_BORDER_WIDTH_3D, cBot);
  // top-right corner
  buffInsert3DCorner(
      buff,
      buffWidth,
      x + w - config::CELL_BORDER_WIDTH_3D,
      y,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      cTop,
      cMid,
      cBot);
  // bottom-left corner
  buffInsert3DCorner(
      buff,
      buffWidth,
      x,
      y + h - config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      cTop,
      cMid,
      cBot);
}

void BufferCompositor::buffInsert3DCorner(
    std::vector<uint32_t> &buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t cTop,
    const uint32_t cMid,
    const uint32_t cBot)
{
  for (int row = y; row < y + h; ++row)
  {
    for (int col = x; col < x + w; ++col)
    {
      const auto idx = row * buffWidth + col;

      const int relY = row - y;
      const int relX = col - x;

      uint32_t color;
      const int diagonal = -relX + h - 1;
      if (relY == diagonal)
      {
        color = cMid;
      }
      else if (relY < diagonal)
      {
        color = cTop;
      }
      else
      {
        color = cBot;
      }

      buff[idx] = color;
    }
  }
}

void BufferCompositor::buffInsertDigit(
    std::vector<uint32_t> &buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const int n,
    const int c)
{
  int segmentWidth = 0.15 * 0.6 * config::CELL_PIXEL_SIZE;

  const int leftX = x;
  const int rightX = x + w - segmentWidth;

  const int topVertY = y;
  const int botVertY = y + h / 2;

  const int horizTopY = y;
  const int horizMidY = y + h / 2 - segmentWidth / 2;
  const int horizBotY = y + h - segmentWidth;

  const auto digitSegments = intToDigitSegments(n);

  if (digitSegments.topMiddle)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, leftX, horizTopY, w, segmentWidth, c);
  }
  if (digitSegments.topLeft)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, leftX, topVertY, segmentWidth, h / 2, c);
  }
  if (digitSegments.topRight)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, rightX, topVertY, segmentWidth, h / 2, c);
  }
  if (digitSegments.middleMiddle)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, leftX, horizMidY, w, segmentWidth, c);
  }
  if (digitSegments.bottomMiddle)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, leftX, horizBotY, w, segmentWidth, c);
  }
  if (digitSegments.bottomLeft)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, leftX, botVertY, segmentWidth, h / 2, c);
  }
  if (digitSegments.bottomRight)
  {
    BufferCompositor::buffInsertRectangle(buff, buffWidth, rightX, botVertY, segmentWidth, h / 2, c);
  }
}