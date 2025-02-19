#include <SpriteFactory.hpp>
#include <cmath>
#include <config.hpp>
#include <memory>
#include <stdexcept>

SpriteFactory::SpriteFactory(Sprites *spriteObjs) : sprites(spriteObjs)
{
  makeRaisedResetButtonSprite();
  makePressedResetButtonSprite();
  makeRaisedButtonSprite();
  makePressedButtonSprite();

  makeEmptyCellSprite();
  makeHiddenCellSprite();
  makeFlaggedCellSprite();
  makeMineCellSprite();
  makeClickedMineCellSprite();
  makeMineCellWithRedXSprite();

  makeOneSprite();
  makeNumericSprite(sprites->two, 2, config::GREEN);
  makeNumericSprite(sprites->three, 3, config::RED);
  makeNumericSprite(sprites->four, 4, config::DARK_BLUE);
  makeNumericSprite(sprites->five, 5, config::DARK_RED);
  makeNumericSprite(sprites->six, 6, config::TURQUOISE);
  makeNumericSprite(sprites->seven, 7, config::PURPLE);
  makeNumericSprite(sprites->eight, 8, config::DARK_GREY);

  sprites->intToSpriteMap = {
      {0, sprites->empty},
      {1, sprites->one},
      {2, sprites->two},
      {3, sprites->three},
      {4, sprites->four},
      {5, sprites->five},
      {6, sprites->six},
      {7, sprites->seven},
      {8, sprites->eight},
  };
}

std::unique_ptr<SpriteFactory::Sprites> SpriteFactory::createSprites()
{
  auto spriteObjs = std::make_unique<Sprites>();

  SpriteFactory spriteFactory(spriteObjs.get());

  return spriteObjs;
}

void SpriteFactory::buffInsertInterface(uint32_t *buff, const int buffWidth, const int buffSize)
{
  // base
  std::fill_n(buff, buffSize, config::GREY);

  // border
  buffInsert3DBorder(
      buff,
      buffWidth,
      0,
      0,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::GAME_WINDOW_PIXEL_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);

  buffInsert3DBorder(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::GAME_WINDOW_PIXEL_HEIGHT - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);

  // interface / game separator
  buffInsert3DBorder(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::FRAME_WIDTH,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  // fill overlap
  buffInsertRectangle(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + config::CELL_BORDER_WIDTH_3D,
      config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::FRAME_WIDTH - 2 * config::CELL_BORDER_WIDTH_3D,
      config::GREY);
  // top-left corner
  buffInsert3DCorner(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
  // top-right corner
  buffInsertRectangle(
      buff,
      buffWidth,
      config::GAME_WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::LIGHT_GREY);
  // bottom-left corner
  buffInsertRectangle(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY);
  // bottom-right corner
  buffInsert3DCorner(
      buff,
      buffWidth,
      config::GAME_WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
};

void SpriteFactory::buffInsertRemainingFlags(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const int n)
{
  buffInsertRectangle(buff, buffWidth, x, y, w, h, config::BLACK);

  const int leftDigit = n / 100;
  const int middleDigit = (n % 100) / 10;
  const int rightDigit = (n % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::REMAINING_FLAGS_PAD;
  while ((w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (w - 4 * pad) / 3;
  const int digitHeight = h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = x + pad + i * (digitWidth + pad);
    const int digitY = y + pad;

    buffInsertDigit(buff, buffWidth, digitX, digitY, digitWidth, digitHeight, digits[i], config::RED);
  }
};

void SpriteFactory::buffInsertTimer(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const int t)
{
  buffInsertRectangle(buff, buffWidth, x, y, w, h, config::BLACK);

  const int leftDigit = t / 100;
  const int middleDigit = (t % 100) / 10;
  const int rightDigit = (t % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::REMAINING_FLAGS_PAD;
  while ((w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (w - 4 * pad) / 3;
  const int digitHeight = h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = x + pad + i * (digitWidth + pad);
    const int digitY = y + pad;

    buffInsertDigit(buff, buffWidth, digitX, digitY, digitWidth, digitHeight, digits[i], config::RED);
  }
}

void SpriteFactory::copySprite(uint32_t *buff, const uint32_t *sprite, const int spriteWidth, const int x, const int y)
{
  for (int row = 0; row < spriteWidth; ++row)
  {
    const auto sourceRow = sprite + row * spriteWidth;
    const auto sourceRowEnd = sprite + row * spriteWidth + spriteWidth;
    const auto destinationRow = buff + rowColToWindowIndex(row + y, x);
    std::copy(sourceRow, sourceRowEnd, destinationRow);
  }
}

void SpriteFactory::copySprite(
    std::unique_ptr<uint32_t[]> &buff,
    const uint32_t *sprite,
    const int spriteWidth,
    const int x,
    const int y)
{
  copySprite(buff.get(), sprite, spriteWidth, x, y);
}

// private

void SpriteFactory::makeRaisedResetButtonSprite()
{
  auto &buff = sprites->raisedResetButton;
  buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  buffInsert3DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  buffInsertFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  // buffInsertFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  buffInsertFaceFrown(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  buffInsertFaceAliveEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
};

void SpriteFactory::makePressedResetButtonSprite()
{
  auto &buff = sprites->pressedResetButton;
  buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  buffInsert2DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::DARK_GREY);
  buffInsertFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  buffInsertFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  buffInsertFaceAliveEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
};

void SpriteFactory::makeRaisedButtonSprite()
{
  auto &buff = sprites->raisedButton;
  buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  buffInsert3DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
}

void SpriteFactory::makePressedButtonSprite()
{
  auto &buff = sprites->pressedButton;
  buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  buffInsert2DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::DARK_GREY);
}

void SpriteFactory::makeEmptyCellSprite()
{
  auto &buff = sprites->empty;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
}

void SpriteFactory::makeHiddenCellSprite()
{
  auto &buff = sprites->hidden;

  // base
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);

  buffInsert3DBorder(
      buff,
      config::CELL_PIXEL_SIZE,
      0,
      0,
      config::CELL_PIXEL_SIZE,
      config::CELL_PIXEL_SIZE,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
}

void SpriteFactory::makeFlaggedCellSprite()
{

  const int totalFlagPoleHeight = 0.55 * config::CELL_PIXEL_SIZE;

  const int flagPoleBottomY = config::CELL_PIXEL_SIZE - ((config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2);

  auto &buff = sprites->flag;
  std::copy(sprites->hidden, sprites->hidden + cellSpriteSize, buff);

  // bottom base rectangle
  const int bottomBaseRectHeight = 0.1 * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectWidth = 0.5 * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectX = (config::CELL_PIXEL_SIZE - bottomBaseRectWidth) / 2;
  const int bottomBaseRectY = flagPoleBottomY - bottomBaseRectHeight;
  buffInsertRectangle(
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
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, topBaseRectX, topBaseRectY, topBaseRectWidth, topBaseRectHeight, config::BLACK);

  // pole
  const int poleWidth = 0.05 * config::CELL_PIXEL_SIZE;
  const int poleX = (config::CELL_PIXEL_SIZE - poleWidth) / 2;
  const int poleY = (config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2;
  buffInsertRectangle(buff, config::CELL_PIXEL_SIZE, poleX, poleY, poleWidth, totalFlagPoleHeight, config::BLACK);

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
        buff[buffY * config::CELL_PIXEL_SIZE + buffX] = config::RED;
      }
    }
  }
}

void SpriteFactory::makeMineCellSprite()
{
  auto &buff = sprites->mine;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  buffInsertMine(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeClickedMineCellSprite()
{
  auto &buff = sprites->clickedMine;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::RED);
  buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  buffInsertMine(buff, config::CELL_PIXEL_SIZE);
};

void SpriteFactory::makeMineCellWithRedXSprite()
{
  auto &buff = sprites->redXMine;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  buffInsertMine(buff, config::CELL_PIXEL_SIZE);
  buffInsertRedX(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeNumericSprite(uint32_t *buff, const int n, const uint32_t c)
{
  std::copy(sprites->empty, sprites->empty + cellSpriteSize, buff);
  buffInsertDigit(
      buff,
      config::CELL_PIXEL_SIZE,
      config::CELL_PIXEL_SIZE / 2 - NUMERIC_SPRITE_WIDTH / 2,
      config::CELL_PIXEL_SIZE / 2 - NUMERIC_SPRITE_HEIGHT / 2,
      NUMERIC_SPRITE_WIDTH,
      NUMERIC_SPRITE_HEIGHT,
      n,
      c);
};

void SpriteFactory::makeOneSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT];
  std::fill_n(sprite, NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT, config::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int baseWidth = NUMERIC_SPRITE_WIDTH;
  const int baseLeftPad = (NUMERIC_SPRITE_HEIGHT - baseWidth) / 2;
  buffInsertRectangle(
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
  buffInsertRectangle(sprite, NUMERIC_SPRITE_HEIGHT, stemLeftPad, 0, stemWidth, NUMERIC_SPRITE_HEIGHT, config::BLUE);

  // topper
  const int topperWidth = 0.2 * NUMERIC_SPRITE_HEIGHT;
  const int topperHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int topperX = stemLeftPad - topperWidth;
  buffInsertRectangle(sprite, NUMERIC_SPRITE_HEIGHT, topperX, 0, topperWidth, topperHeight, config::BLUE);

  auto &buff = sprites->one;
  std::copy(sprites->empty, sprites->empty + cellSpriteSize, buff);
  for (int i = 0; i < NUMERIC_SPRITE_HEIGHT; ++i)
  {
    const auto spriteStart = sprite + i * NUMERIC_SPRITE_HEIGHT;
    const auto spriteEnd = sprite + i * NUMERIC_SPRITE_HEIGHT + NUMERIC_SPRITE_HEIGHT;
    const int buffIdx = ((i + NUMERIC_SPRITE_PAD) * config::CELL_PIXEL_SIZE) + NUMERIC_SPRITE_PAD;
    std::copy(spriteStart, spriteEnd, buff + buffIdx);
  }
}

void SpriteFactory::buffInsertRectangle(
    uint32_t *buff,
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
    std::fill_n(buff + index, w, c);
  }
};

void SpriteFactory::buffInsert2DBorder(
    uint32_t *buff,
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

void SpriteFactory::buffInsert3DBorder(
    uint32_t *buff,
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

void SpriteFactory::buffInsert3DCorner(
    uint32_t *buff,
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

void SpriteFactory::buffInsertDigit(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const int n,
    const int c)
{
  const int segmentWidth = 0.15 * NUMERIC_SPRITE_HEIGHT;

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
    buffInsertRectangle(buff, buffWidth, leftX, horizTopY, w, segmentWidth, c);
  }
  if (digitSegments.topLeft)
  {
    buffInsertRectangle(buff, buffWidth, leftX, topVertY, segmentWidth, h / 2, c);
  }
  if (digitSegments.topRight)
  {
    buffInsertRectangle(buff, buffWidth, rightX, topVertY, segmentWidth, h / 2, c);
  }
  if (digitSegments.middleMiddle)
  {
    buffInsertRectangle(buff, buffWidth, leftX, horizMidY, w, segmentWidth, c);
  }
  if (digitSegments.bottomMiddle)
  {
    buffInsertRectangle(buff, buffWidth, leftX, horizBotY, w, segmentWidth, c);
  }
  if (digitSegments.bottomLeft)
  {
    buffInsertRectangle(buff, buffWidth, leftX, botVertY, segmentWidth, h / 2, c);
  }
  if (digitSegments.bottomRight)
  {
    buffInsertRectangle(buff, buffWidth, rightX, botVertY, segmentWidth, h / 2, c);
  }
}

void SpriteFactory::buffInsertMine(uint32_t *buff, const int buffWidth)
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

void SpriteFactory::buffInsertRedX(uint32_t *buff, const int buffWidth)
{
  const int size = buffWidth;
  const double mineCenter = size / 2;
  const double drawRadius = size / 2 * 0.9;
  const double diagThickness = 4;
  const double diagRadius = diagThickness / std::sqrt(2);

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {

      const double dx = x - mineCenter;
      const double dy = y - mineCenter;

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool positiveDiag = dy <= dx + diagRadius && dy >= dx - diagRadius;
      const bool negativeDiag = dy <= -dx + diagRadius && dy >= -dx - diagRadius;

      if (positiveDiag || negativeDiag)
      {
        buff[y * size + x] = config::RED;
      }
    }
  }
}

void SpriteFactory::buffInsertFaceBase(uint32_t *buff, const int buffWidth)
{
  const int size = buffWidth;
  const double drawRadius = size / 2 * 0.9;
  const double faceCenter = size / 2;
  const double faceRadius = size / 2 * 0.6;
  const double faceRadiusSqrd = faceRadius * faceRadius;
  const double outlineRadiusSqrd = faceRadiusSqrd * 1.3;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - faceCenter;
      const double dy = y - faceCenter;

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool faceCircle = dx * dx + dy * dy <= faceRadiusSqrd;

      if (faceCircle)
      {
        buff[y * size + x] = config::YELLOW;
        continue;
      }

      const bool outlineCircle = dx * dx + dy * dy <= outlineRadiusSqrd;
      if (outlineCircle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void SpriteFactory::buffInsertFaceSmile(uint32_t *buff, const int buffWidth)
{
  const int size = buffWidth;
  const double drawRadius = size / 2 * 0.9;
  const double faceCenter = size / 2;
  const double smileBigRadius = size / 2 * 0.4;
  const double smileBigRadiusSqrd = smileBigRadius * smileBigRadius;
  const double smileSmallRadius = size / 2 * 0.3;
  const double smileSmallRadiusSqrd = smileSmallRadius * smileSmallRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - faceCenter;
      const double dy = y - faceCenter;
      const double theta = std::atan2(dy, dx);

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool smileBigCircle = dx * dx + dy * dy <= smileBigRadiusSqrd;
      const bool smileSmallCircle = dx * dx + dy * dy <= smileSmallRadiusSqrd;
      const bool smileAngle = (theta >= 0) && (theta <= (M_PI * 2));

      if (smileBigCircle && !smileSmallCircle && smileAngle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void SpriteFactory::buffInsertFaceFrown(uint32_t *buff, const int buffWidth)
{
  const int size = buffWidth;
  const double drawRadius = size / 2 * 0.9;
  const double faceCenter = size / 2;
  const double frownBigRadius = size / 2 * 0.4;
  const double frownBigRadiusSqrd = frownBigRadius * frownBigRadius;
  const double frownSmallRadius = size / 2 * 0.3;
  const double frownSmallRadiusSqrd = frownSmallRadius * frownSmallRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - faceCenter;
      const double dy = y - faceCenter;
      const double theta = std::atan2(dy, dx);

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool smileBigCircle = dx * dx + dy * dy <= frownBigRadiusSqrd;
      const bool smileSmallCircle = dx * dx + dy * dy <= frownSmallRadiusSqrd;
      const bool smileAngle = (theta >= 0) && (theta <= (M_PI * 2));

      if (smileBigCircle && !smileSmallCircle && smileAngle)
      {
        buff[(size - y + (size / 5)) * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void SpriteFactory::buffInsertFaceAliveEyes(uint32_t *buff, const int buffWidth)
{
  const int size = buffWidth;
  const double drawRadius = size / 2 * 0.9;
  const double leftEyeX = size * 13 / 32;
  const double leftEyeY = size * 3 / 8;
  const double rightEyeX = size * 19 / 32;
  const double rightEyeY = size * 3 / 8;
  const double eyeRadius = size / 2 * 0.1;
  const double eyeRadiusSqrd = eyeRadius * eyeRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double leftDx = x - leftEyeX;
      const double leftDy = y - leftEyeY;

      const double rightDx = x - rightEyeX;
      const double rightDy = y - rightEyeY;

      const bool leftInBounds = leftDx * leftDx + leftDy * leftDy <= drawRadius * drawRadius;
      const bool rightInBounds = rightDx * rightDx + rightDy * rightDy <= drawRadius * drawRadius;
      if (!leftInBounds && !rightInBounds)
      {
        continue;
      }

      const bool leftCircle = leftDx * leftDx + leftDy * leftDy <= eyeRadiusSqrd;
      const bool rightCircle = rightDx * rightDx + rightDy * rightDy <= eyeRadiusSqrd;

      if (leftCircle || rightCircle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void SpriteFactory::buffInsertFaceDeadEyes(uint32_t *buff, const int buffWidth) {};
void SpriteFactory::buffInsertFaceShades(uint32_t *buff, const int buffWidth) {};

SpriteFactory::DigitSegments SpriteFactory::intToDigitSegments(const int n)
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

int SpriteFactory::rowColToWindowIndex(const int row, const int col)
{
  return row * config::GAME_WINDOW_PIXEL_WIDTH + col;
}

int SpriteFactory::rowColToCellIndex(const int row, const int col) { return row * config::CELL_PIXEL_SIZE + col; }

std::pair<int, int> SpriteFactory::rowColToPixelPoint(const int row, const int col)
{
  return {col * config::CELL_PIXEL_SIZE, row * config::CELL_PIXEL_SIZE};
};