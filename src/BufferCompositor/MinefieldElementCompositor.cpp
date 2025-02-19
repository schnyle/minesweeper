#include <MinefieldElementCompositor.hpp>
#include <cstdint>

void MinefieldElementCompositor::buffInsertMine(uint32_t *buff, const int buffWidth)
{
  const int size = buffWidth;
  const double mineCenter = size / 2;
  const double drawRadius = size / 3;
  const double glintCenter = 7 * size / 16;
  const double glintRadius = size / 20;
  const double mineRadius = size / 4;
  const double mineRadiusSqrd = mineRadius * mineRadius;
  const double spikeThickness = 3;
  const double spikeRadius = spikeThickness / std::sqrt(2);

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double glintDx = x - glintCenter;
      const double glintDy = y - glintCenter;
      const bool glint = glintDx * glintDx + glintDy * glintDy <= glintRadius * glintRadius;

      if (glint)
      {
        buff[y * size + x] = config::WHITE;
        continue;
      }

      const double dx = x - mineCenter;
      const double dy = y - mineCenter;

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool circle = dx * dx + dy * dy <= mineRadiusSqrd;
      const bool verticalLine = std::abs(dx) <= spikeThickness / 2;
      const bool horizontalLine = std::abs(dy) <= spikeThickness / 2;
      const bool positiveDiag = dy <= dx + spikeRadius && dy >= dx - spikeRadius;
      const bool negativeDiag = dy <= -dx + spikeRadius && dy >= -dx - spikeRadius;

      if (circle || verticalLine || horizontalLine || positiveDiag || negativeDiag)
      {
        buff[y * size + x] = config::BLACK;
      }
    }
  }
}

void MinefieldElementCompositor::buffInsertFlag(uint32_t *buff, const int buffWidth)
{
  const int totalFlagPoleHeight = 0.55 * config::CELL_PIXEL_SIZE;

  const int flagPoleBottomY = config::CELL_PIXEL_SIZE - ((config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2);

  // bottom base rectangle
  const int bottomBaseRectHeight = 0.1 * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectWidth = 0.5 * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectX = (config::CELL_PIXEL_SIZE - bottomBaseRectWidth) / 2;
  const int bottomBaseRectY = flagPoleBottomY - bottomBaseRectHeight;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::CELL_PIXEL_SIZE,
      bottomBaseRectX,
      bottomBaseRectY,
      bottomBaseRectWidth,
      bottomBaseRectHeight,
      config::BLACK);

  // top base rectangle
  const int topBaseRectHeight = 0.05 * config::CELL_PIXEL_SIZE;
  const int topBaseRectWidth = 0.33 * config::CELL_PIXEL_SIZE;
  const int topBaseRectX = (config::CELL_PIXEL_SIZE - topBaseRectWidth) / 2;
  const int topBaseRectY = flagPoleBottomY - bottomBaseRectHeight - topBaseRectHeight;
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, topBaseRectX, topBaseRectY, topBaseRectWidth, topBaseRectHeight, config::BLACK);

  // pole
  const int poleWidth = 0.05 * config::CELL_PIXEL_SIZE;
  const int poleX = (config::CELL_PIXEL_SIZE - poleWidth) / 2;
  const int poleY = (config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2;
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, poleX, poleY, poleWidth, totalFlagPoleHeight, config::BLACK);

  // flag
  const int flagSize = 0.3 * config::CELL_PIXEL_SIZE;
  const int flagX = poleX + poleWidth - flagSize;
  const int flagY = poleY;
  const double flagSlope = 0.66;
  for (int y = 0; y < flagSize; ++y)
  {
    const int buffY = y + flagY;
    for (int x = 0; x < flagSize; ++x)
    {
      if (y <= (flagSlope * x) + (flagSize / 2) - 1 && y >= -(flagSlope * x) + (flagSize / 2) + 1)
      {
        const int buffX = x + flagX;
        buff[buffY * buffWidth + buffX] = config::RED;
      }
    }
  }
}

void MinefieldElementCompositor::buffInsertOne(uint32_t *buff, const int buffWidth)
{
  uint32_t sprite[NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT];
  std::fill_n(sprite, NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT, config::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int baseWidth = NUMERIC_SPRITE_WIDTH;
  const int baseLeftPad = (NUMERIC_SPRITE_HEIGHT - baseWidth) / 2;
  BufferCompositor::buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_HEIGHT,
      baseLeftPad,
      NUMERIC_SPRITE_HEIGHT - baseHeight,
      baseWidth,
      baseHeight,
      config::BLUE);

  // stem
  const int stemWidth = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int stemLeftPad = (NUMERIC_SPRITE_HEIGHT - stemWidth) / 2;
  BufferCompositor::buffInsertRectangle(
      sprite, NUMERIC_SPRITE_HEIGHT, stemLeftPad, 0, stemWidth, NUMERIC_SPRITE_HEIGHT, config::BLUE);

  // topper
  const int topperWidth = 0.2 * NUMERIC_SPRITE_HEIGHT;
  const int topperHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int topperX = stemLeftPad - topperWidth;
  BufferCompositor::buffInsertRectangle(
      sprite, NUMERIC_SPRITE_HEIGHT, topperX, 0, topperWidth, topperHeight, config::BLUE);

  for (int i = 0; i < NUMERIC_SPRITE_HEIGHT; ++i)
  {
    const auto spriteStart = sprite + i * NUMERIC_SPRITE_HEIGHT;
    const auto spriteEnd = sprite + i * NUMERIC_SPRITE_HEIGHT + NUMERIC_SPRITE_HEIGHT;
    const int buffIdx = ((i + NUMERIC_SPRITE_PAD) * buffWidth) + NUMERIC_SPRITE_PAD;
    std::copy(spriteStart, spriteEnd, buff + buffIdx);
  }
}