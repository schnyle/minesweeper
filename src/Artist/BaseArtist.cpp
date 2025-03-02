#include <BaseArtist.hpp>
#include <Rect.h>
#include <algorithm>
#include <config.hpp>
#include <cstdint>
#include <stdexcept>

// public

void BaseArtist::draw2DCellBase(std::vector<uint32_t> &buff, const int width)
{
  const int height = buff.size() / width;
  const Rect rect = {0, 0, width, height};
  drawRectangle(buff, width, rect, config::GREY);
  draw2DBorder(buff, width, rect, config::DARK_GREY);
};

void BaseArtist::draw3DCellBase(std::vector<uint32_t> &buff, const int width)
{
  const int height = buff.size() / width;
  const Rect rect = {0, 0, width, height};
  drawRectangle(buff, width, rect, config::GREY);
  draw3DBorder(buff, width, rect, config::LIGHT_GREY, config::GREY, config::DARK_GREY);
};

void BaseArtist::draw2DBorder(std::vector<uint32_t> &buff, const int width, const Rect rect, const uint32_t c)
{
  // left edge
  drawRectangle(buff, width, {rect.x, rect.y, config::CELL_BORDER_WIDTH_2D, rect.h}, c);
  // top edge
  drawRectangle(buff, width, {rect.x, rect.y, rect.w, config::CELL_BORDER_WIDTH_2D}, c);
  // right edge
  drawRectangle(buff, width, {rect.w - config::CELL_BORDER_WIDTH_2D, rect.y, config::CELL_BORDER_WIDTH_2D, rect.h}, c);
  // left edge
  drawRectangle(buff, width, {rect.x, rect.h - config::CELL_BORDER_WIDTH_2D, rect.w, config::CELL_BORDER_WIDTH_2D}, c);
}

void BaseArtist::draw3DBorder(
    std::vector<uint32_t> &buff,
    const int width,
    const Rect rect,
    const uint32_t cTop,
    const uint32_t cMid,
    const uint32_t cBot)
{
  // left edge
  drawRectangle(buff, width, {rect.x, rect.y, config::CELL_BORDER_WIDTH_3D, rect.h}, cTop);
  // top edge
  drawRectangle(buff, width, {rect.x, rect.y, rect.w, config::CELL_BORDER_WIDTH_3D}, cTop);
  // right edge
  drawRectangle(
      buff, width, {rect.x + rect.w - config::CELL_BORDER_WIDTH_3D, rect.y, config::CELL_BORDER_WIDTH_3D, rect.h}, cBot);
  // bottom edge
  drawRectangle(
      buff, width, {rect.x, rect.y + rect.h - config::CELL_BORDER_WIDTH_3D, rect.w, config::CELL_BORDER_WIDTH_3D}, cBot);
  // top-right corner
  draw3DCorner(
      buff,
      width,
      {rect.x + rect.w - config::CELL_BORDER_WIDTH_3D,
       rect.y,
       config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D},
      cTop,
      cMid,
      cBot);
  // bottom-left corner
  draw3DCorner(
      buff,
      width,
      {rect.x,
       rect.y + rect.h - config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D,
       config::CELL_BORDER_WIDTH_3D},
      cTop,
      cMid,
      cBot);
}

void BaseArtist::draw3DCorner(
    std::vector<uint32_t> &buff,
    const int width,
    const Rect rect,
    const uint32_t cTop,
    const uint32_t cMid,
    const uint32_t cBot)
{
  for (int row = rect.y; row < rect.y + rect.h; ++row)
  {
    for (int col = rect.x; col < rect.x + rect.w; ++col)
    {
      const auto idx = row * width + col;

      const int relY = row - rect.y;
      const int relX = col - rect.x;

      uint32_t color;
      const int diagonal = -relX + rect.h - 1;
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

void BaseArtist::drawRectangle(std::vector<uint32_t> &buff, const int width, const Rect rect, const uint32_t c)
{
  for (int row = rect.y; row < rect.y + rect.h; ++row)
  {
    const int index = row * width + rect.x;
    std::fill_n(buff.begin() + index, rect.w, c);
  }
};

void BaseArtist::drawDigit(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n, const int c)
{
  int segmentWidth = 0.15 * 0.6 * config::CELL_PIXEL_SIZE;

  const int leftX = rect.x;
  const int rightX = rect.x + rect.w - segmentWidth;

  const int topVertY = rect.y;
  const int botVertY = rect.y + rect.h / 2;

  const int horizTopY = rect.y;
  const int horizMidY = rect.y + rect.h / 2 - segmentWidth / 2;
  const int horizBotY = rect.y + rect.h - segmentWidth;

  const auto digitSegments = intToDigitSegments(n);

  if (digitSegments.topMiddle)
  {
    BaseArtist::drawRectangle(buff, width, {leftX, horizTopY, rect.w, segmentWidth}, c);
  }
  if (digitSegments.topLeft)
  {
    BaseArtist::drawRectangle(buff, width, {leftX, topVertY, segmentWidth, rect.h / 2}, c);
  }
  if (digitSegments.topRight)
  {
    BaseArtist::drawRectangle(buff, width, {rightX, topVertY, segmentWidth, rect.h / 2}, c);
  }
  if (digitSegments.middleMiddle)
  {
    BaseArtist::drawRectangle(buff, width, {leftX, horizMidY, rect.w, segmentWidth}, c);
  }
  if (digitSegments.bottomMiddle)
  {
    BaseArtist::drawRectangle(buff, width, {leftX, horizBotY, rect.w, segmentWidth}, c);
  }
  if (digitSegments.bottomLeft)
  {
    BaseArtist::drawRectangle(buff, width, {leftX, botVertY, segmentWidth, rect.h / 2}, c);
  }
  if (digitSegments.bottomRight)
  {
    BaseArtist::drawRectangle(buff, width, {rightX, botVertY, segmentWidth, rect.h / 2}, c);
  }
}

void BaseArtist::drawX(
    std::vector<uint32_t> &buff,
    const int width,
    const uint32_t c,
    const double xCenter,
    const double yCenter,
    const double xSize,
    const double xThickness)
{
  const int size = width;
  const double xRadius = xThickness / std::sqrt(2);

  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      const int dx = x - xCenter;
      const int dy = y - yCenter;

      if (std::abs(dx) > xSize / 2 || std::abs(dy) > xSize / 2)
      {
        continue;
      }

      const bool positiveDiag = dy <= dx + xRadius && dy >= dx - xRadius;
      const bool negativeDiag = dy <= -dx + xRadius && dy >= -dx - xRadius;

      if (positiveDiag || negativeDiag)
      {
        buff[y * size + x] = c;
      }
    }
  }
}

// private

BaseArtist::DigitSegments BaseArtist::intToDigitSegments(const int n)
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