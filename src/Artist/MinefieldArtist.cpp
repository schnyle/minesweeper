#include <FaceArtist.hpp>
#include <MinefieldArtist.hpp>
#include <Sprites.hpp>
#include <cstdint>

// private static ints

int MinefieldArtist::NUMERIC_SPRITE_HEIGHT = 0.6 * config::getSettings().getCellPixelSize();
int MinefieldArtist::NUMERIC_SPRITE_WIDTH = NUMERIC_SPRITE_HEIGHT / 2;
int MinefieldArtist::NUMERIC_SPRITE_PAD = (config::getSettings().getCellPixelSize() - NUMERIC_SPRITE_HEIGHT) / 2;

// public

void MinefieldArtist::updateMinefield(std::vector<uint32_t> &buff, const int width, Minesweeper &gameState)
{
  static int gameAreaX = config::getSettings().getGridAreaPadX() + config::FRAME_WIDTH;
  static int gameAreaY = config::getSettings().getGridAreaPadY() + config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

  for (int row = 0; row < config::getSettings().getGridHeight(); ++row)
  {
    for (int col = 0; col < config::getSettings().getGridWidth(); ++col)
    {
      const int cellIndex = row * config::getSettings().getGridWidth() + col;
      const auto &sprite = getCellSprite(gameState, cellIndex);

      // could cache these values so they are only calculaated once during construction/init...
      const int x = gameAreaX + col * width;
      const int y = gameAreaY + row * width;
      Sprites::getInstance().copy(sprite, buff, width, x, y);
    }
  }
};

void MinefieldArtist::drawEmptyCellSprite(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);
};

void MinefieldArtist::drawHiddenCellSprite(std::vector<uint32_t> &buff, const int width)
{
  draw3DCellBase(buff, width);
};

void MinefieldArtist::drawFlaggedCellSprite(std::vector<uint32_t> &buff, const int width)
{
  draw3DCellBase(buff, width);
  drawFlag(buff, width);
}

void MinefieldArtist::drawMineCellSprite(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);
  drawMine(buff, width);
};

void MinefieldArtist::drawMineCellRedXSprite(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);
  drawMine(buff, width);
  drawX(buff, width, config::Colors::RED, width / 2, width / 2, width * 0.7, 4);
}

void MinefieldArtist::drawClickedMineCellSprite(std::vector<uint32_t> &buff, const int width)
{
  const int height = buff.size() / width;
  const Rect rect = {0, 0, width, height};
  drawRectangle(buff, width, rect, config::Colors::RED);
  draw2DBorder(buff, width, rect, config::Colors::DARK_GREY);
  drawMine(buff, width);
}

void MinefieldArtist::drawNumericSprite(std::vector<uint32_t> &buff, const int width, const int n, const uint32_t c)
{
  draw2DCellBase(buff, width);
  if (n != 1)
  {
    drawDigit(
        buff,
        width,
        {width / 2 - NUMERIC_SPRITE_WIDTH / 2,
         width / 2 - NUMERIC_SPRITE_HEIGHT / 2,
         NUMERIC_SPRITE_WIDTH,
         NUMERIC_SPRITE_HEIGHT},
        n,
        c);
  }
  else
  {
    drawOne(buff, width);
  }
};

// private

void MinefieldArtist::drawMine(std::vector<uint32_t> &buff, const int width)
{
  const int size = width;
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
        buff[y * size + x] = config::Colors::WHITE;
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
        buff[y * size + x] = config::Colors::BLACK;
      }
    }
  }
}

void MinefieldArtist::drawFlag(std::vector<uint32_t> &buff, const int width)
{
  const int totalFlagPoleHeight = 0.55 * config::getSettings().getCellPixelSize();

  const int flagPoleBottomY = config::getSettings().getCellPixelSize() -
                              ((config::getSettings().getCellPixelSize() - totalFlagPoleHeight) / 2);

  // bottom base rectangle
  const int bottomBaseRectHeight = 0.1 * config::getSettings().getCellPixelSize();
  const int bottomBaseRectWidth = 0.5 * config::getSettings().getCellPixelSize();
  const int bottomBaseRectX = (config::getSettings().getCellPixelSize() - bottomBaseRectWidth) / 2;
  const int bottomBaseRectY = flagPoleBottomY - bottomBaseRectHeight;
  BaseArtist::drawRectangle(
      buff,
      config::getSettings().getCellPixelSize(),
      {bottomBaseRectX, bottomBaseRectY, bottomBaseRectWidth, bottomBaseRectHeight},
      config::Colors::BLACK);

  // top base rectangle
  const int topBaseRectHeight = 0.05 * config::getSettings().getCellPixelSize();
  const int topBaseRectWidth = 0.33 * config::getSettings().getCellPixelSize();
  const int topBaseRectX = (config::getSettings().getCellPixelSize() - topBaseRectWidth) / 2;
  const int topBaseRectY = flagPoleBottomY - bottomBaseRectHeight - topBaseRectHeight;
  BaseArtist::drawRectangle(
      buff,
      config::getSettings().getCellPixelSize(),
      {topBaseRectX, topBaseRectY, topBaseRectWidth, topBaseRectHeight},
      config::Colors::BLACK);

  // pole
  const int poleWidth = 0.05 * config::getSettings().getCellPixelSize();
  const int poleX = (config::getSettings().getCellPixelSize() - poleWidth) / 2;
  const int poleY = (config::getSettings().getCellPixelSize() - totalFlagPoleHeight) / 2;
  BaseArtist::drawRectangle(
      buff,
      config::getSettings().getCellPixelSize(),
      {poleX, poleY, poleWidth, totalFlagPoleHeight},
      config::Colors::BLACK);

  // flag
  const int flagSize = 0.3 * config::getSettings().getCellPixelSize();
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
        buff[buffY * width + buffX] = config::Colors::RED;
      }
    }
  }
}

void MinefieldArtist::drawOne(std::vector<uint32_t> &buff, const int width)
{
  std::vector<uint32_t> sprite;
  sprite.resize(NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT);
  std::fill_n(sprite.begin(), NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT, config::Colors::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int baseWidth = NUMERIC_SPRITE_WIDTH;
  const int baseLeftPad = (NUMERIC_SPRITE_HEIGHT - baseWidth) / 2;
  BaseArtist::drawRectangle(
      sprite,
      NUMERIC_SPRITE_HEIGHT,
      {baseLeftPad, NUMERIC_SPRITE_HEIGHT - baseHeight, baseWidth, baseHeight},
      config::Colors::BLUE);

  // stem
  const int stemWidth = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int stemLeftPad = (NUMERIC_SPRITE_HEIGHT - stemWidth) / 2;
  BaseArtist::drawRectangle(
      sprite, NUMERIC_SPRITE_HEIGHT, {stemLeftPad, 0, stemWidth, NUMERIC_SPRITE_HEIGHT}, config::Colors::BLUE);

  // topper
  const int topperWidth = 0.2 * NUMERIC_SPRITE_HEIGHT;
  const int topperHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int topperX = stemLeftPad - topperWidth;
  BaseArtist::drawRectangle(
      sprite, NUMERIC_SPRITE_HEIGHT, {topperX, 0, topperWidth, topperHeight}, config::Colors::BLUE);

  for (int i = 0; i < NUMERIC_SPRITE_HEIGHT; ++i)
  {
    const auto spriteStart = sprite.begin() + i * NUMERIC_SPRITE_HEIGHT;
    const auto spriteEnd = sprite.begin() + i * NUMERIC_SPRITE_HEIGHT + NUMERIC_SPRITE_HEIGHT;
    const int buffIdx = ((i + NUMERIC_SPRITE_PAD) * width) + NUMERIC_SPRITE_PAD;
    std::copy(spriteStart, spriteEnd, buff.begin() + buffIdx);
  }
}

const std::vector<uint32_t> &MinefieldArtist::getCellSprite(const Minesweeper &gameState, const int cellIndex)
{
  const auto &[isMine, isHidden, isFlagged, isClicked, nAdjacentMines] = gameState.getMinefield()[cellIndex];

  if (isHidden && !isFlagged)
  {
    return Sprites::getInstance().get()->hidden;
  }
  else if (isHidden && isFlagged && !isMine && gameState.getIsGameOver())
  {
    return Sprites::getInstance().get()->redXMine;
  }
  else if (isHidden && isFlagged)
  {
    return Sprites::getInstance().get()->flag;
  }
  else
  {
    if (isMine)
    {
      return isClicked ? Sprites::getInstance().get()->clickedMine : Sprites::getInstance().get()->mine;
    }
    else
    {
      return Sprites::getInstance().get()->intToSpriteMap.at(nAdjacentMines);
    }
  }
};