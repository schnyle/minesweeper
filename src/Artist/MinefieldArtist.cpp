#include <FaceArtist.hpp>
#include <MinefieldArtist.hpp>
#include <cstdint>
#include <sprites.hpp>

int MinefieldArtist::NUMERIC_SPRITE_HEIGHT = 0.6 * config::CELL_PIXEL_SIZE;
int MinefieldArtist::NUMERIC_SPRITE_WIDTH = NUMERIC_SPRITE_HEIGHT / 2;
int MinefieldArtist::NUMERIC_SPRITE_PAD = (config::CELL_PIXEL_SIZE - NUMERIC_SPRITE_HEIGHT) / 2;

void MinefieldArtist::updateMinefield(std::vector<uint32_t> &buff, const int width, Minesweeper &gameState)
{
  static int gameAreaX = config::GRID_AREA_X_PAD + config::FRAME_WIDTH;
  static int gameAreaY = config::GRID_AREA_Y_PAD + config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

  for (int row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (int col = 0; col < config::GRID_WIDTH; ++col)
    {
      const int cellIndex = row * config::GRID_WIDTH + col;
      const auto &sprite = getCellSprite(gameState, cellIndex);

      // could cache these values so they are only calculated once during construction/init...
      const int x = gameAreaX + col * width;
      const int y = gameAreaY + row * width;
      sprites::copy(sprite, buff, width, x, y);
    }
  }
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

void MinefieldArtist::drawClickedMineCellSprite(std::vector<uint32_t> &buff, const int width)
{
  drawClickedMineCell(buff, width);
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

void MinefieldArtist::drawMineCell(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);
  drawMine(buff, width);
};

void MinefieldArtist::drawMineCellRedXSprite(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);
  drawMine(buff, width);
  FaceArtist::drawX(buff, width, config::RED, width / 2, width / 2, width * 0.7, 4);
}

void MinefieldArtist::drawClickedMineCell(std::vector<uint32_t> &buff, const int width)
{
  const int height = buff.size() / width;
  const Rect rect = {0, 0, width, height};
  drawRectangle(buff, width, rect, config::RED);
  draw2DBorder(buff, width, rect, config::DARK_GREY);
  drawMine(buff, width);
};

void MinefieldArtist::draw2DCellBase(std::vector<uint32_t> &buff, const int width)
{
  const int height = buff.size() / width;
  const Rect rect = {0, 0, width, height};
  drawRectangle(buff, width, rect, config::GREY);
  draw2DBorder(buff, width, rect, config::DARK_GREY);
};

void MinefieldArtist::draw3DCellBase(std::vector<uint32_t> &buff, const int width)
{
  const int height = buff.size() / width;
  const Rect rect = {0, 0, width, height};
  drawRectangle(buff, width, rect, config::GREY);
  draw3DBorder(buff, width, rect, config::LIGHT_GREY, config::GREY, config::DARK_GREY);
};

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

void MinefieldArtist::drawFlag(std::vector<uint32_t> &buff, const int width)
{
  const int totalFlagPoleHeight = 0.55 * config::CELL_PIXEL_SIZE;

  const int flagPoleBottomY = config::CELL_PIXEL_SIZE - ((config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2);

  // bottom base rectangle
  const int bottomBaseRectHeight = 0.1 * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectWidth = 0.5 * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectX = (config::CELL_PIXEL_SIZE - bottomBaseRectWidth) / 2;
  const int bottomBaseRectY = flagPoleBottomY - bottomBaseRectHeight;
  BaseArtist::drawRectangle(
      buff,
      config::CELL_PIXEL_SIZE,
      {bottomBaseRectX, bottomBaseRectY, bottomBaseRectWidth, bottomBaseRectHeight},
      config::BLACK);

  // top base rectangle
  const int topBaseRectHeight = 0.05 * config::CELL_PIXEL_SIZE;
  const int topBaseRectWidth = 0.33 * config::CELL_PIXEL_SIZE;
  const int topBaseRectX = (config::CELL_PIXEL_SIZE - topBaseRectWidth) / 2;
  const int topBaseRectY = flagPoleBottomY - bottomBaseRectHeight - topBaseRectHeight;
  BaseArtist::drawRectangle(
      buff, config::CELL_PIXEL_SIZE, {topBaseRectX, topBaseRectY, topBaseRectWidth, topBaseRectHeight}, config::BLACK);

  // pole
  const int poleWidth = 0.05 * config::CELL_PIXEL_SIZE;
  const int poleX = (config::CELL_PIXEL_SIZE - poleWidth) / 2;
  const int poleY = (config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2;
  BaseArtist::drawRectangle(
      buff, config::CELL_PIXEL_SIZE, {poleX, poleY, poleWidth, totalFlagPoleHeight}, config::BLACK);

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
        buff[buffY * width + buffX] = config::RED;
      }
    }
  }
}

void MinefieldArtist::drawOne(std::vector<uint32_t> &buff, const int width)
{
  std::vector<uint32_t> sprite;
  sprite.resize(NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT);
  std::fill_n(sprite.begin(), NUMERIC_SPRITE_HEIGHT * NUMERIC_SPRITE_HEIGHT, config::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int baseWidth = NUMERIC_SPRITE_WIDTH;
  const int baseLeftPad = (NUMERIC_SPRITE_HEIGHT - baseWidth) / 2;
  BaseArtist::drawRectangle(
      sprite,
      NUMERIC_SPRITE_HEIGHT,
      {baseLeftPad, NUMERIC_SPRITE_HEIGHT - baseHeight, baseWidth, baseHeight},
      config::BLUE);

  // stem
  const int stemWidth = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int stemLeftPad = (NUMERIC_SPRITE_HEIGHT - stemWidth) / 2;
  BaseArtist::drawRectangle(
      sprite, NUMERIC_SPRITE_HEIGHT, {stemLeftPad, 0, stemWidth, NUMERIC_SPRITE_HEIGHT}, config::BLUE);

  // topper
  const int topperWidth = 0.2 * NUMERIC_SPRITE_HEIGHT;
  const int topperHeight = 0.15 * NUMERIC_SPRITE_HEIGHT;
  const int topperX = stemLeftPad - topperWidth;
  BaseArtist::drawRectangle(sprite, NUMERIC_SPRITE_HEIGHT, {topperX, 0, topperWidth, topperHeight}, config::BLUE);

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
    return sprites::get()->hidden;
  }
  else if (isHidden && isFlagged && !isMine && gameState.getIsGameOver())
  {
    return sprites::get()->redXMine;
  }
  else if (isHidden && isFlagged)
  {
    return sprites::get()->flag;
  }
  else
  {
    if (isMine)
    {
      return isClicked ? sprites::get()->clickedMine : sprites::get()->mine;
    }
    else
    {
      return sprites::get()->intToSpriteMap.at(nAdjacentMines);
    }
  }
};