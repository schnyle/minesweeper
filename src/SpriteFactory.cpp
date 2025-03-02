#include <BaseArtist.hpp>
#include <FaceArtist.hpp>
#include <HeaderArtist.hpp>
#include <MinefieldElementArtist.hpp>
#include <SpriteFactory.hpp>
#include <cmath>
#include <config.hpp>
#include <memory>
#include <stdexcept>

SpriteFactory::SpriteFactory(Sprites *spriteObjs) : sprites(spriteObjs)
{
  sprites->raisedResetButton.resize(resetButtonSize);
  sprites->pressedResetButton.resize(resetButtonSize);
  sprites->winnerResetButton.resize(resetButtonSize);
  sprites->loserResetButton.resize(resetButtonSize);
  sprites->raisedConfigButton.resize(resetButtonSize);
  sprites->pressedConfigButton.resize(resetButtonSize);
  sprites->empty.resize(cellSpriteSize);
  sprites->hidden.resize(cellSpriteSize);
  sprites->flag.resize(cellSpriteSize);
  sprites->mine.resize(cellSpriteSize);
  sprites->clickedMine.resize(cellSpriteSize);
  sprites->redXMine.resize(cellSpriteSize);
  sprites->zero.resize(cellSpriteSize);
  sprites->one.resize(cellSpriteSize);
  sprites->two.resize(cellSpriteSize);
  sprites->three.resize(cellSpriteSize);
  sprites->four.resize(cellSpriteSize);
  sprites->five.resize(cellSpriteSize);
  sprites->six.resize(cellSpriteSize);
  sprites->seven.resize(cellSpriteSize);
  sprites->eight.resize(cellSpriteSize);

  makeRaisedResetButtonSprite();
  makePressedResetButtonSprite();
  makeWinnerResetButtonSprite();
  makeLoserResetButtonSprite();
  makeRaisedConfigButtonSprite();
  makePressedConfigButtonSprite();

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

void SpriteFactory::copySprite(
    std::vector<uint32_t> &buff,
    const std::vector<uint32_t> &sprite,
    const int spriteWidth,
    const int x,
    const int y)
{
  for (int row = 0; row < spriteWidth; ++row)
  {
    const auto sourceRow = sprite.begin() + row * spriteWidth;
    const auto sourceRowEnd = sprite.begin() + (row * spriteWidth + spriteWidth);
    const auto destinationRow = buff.begin() + rowColToWindowIndex(row + y, x);
    std::copy(sourceRow, sourceRowEnd, destinationRow);
  }
}

void SpriteFactory::makeRaisedResetButtonSprite()
{
  auto &buff = sprites->raisedResetButton;
  MinefieldElementArtist::draw3DCellBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);

  const double center = config::INFO_PANEL_BUTTONS_HEIGHT * 0.5;
  FaceArtist::drawFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceAliveEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
};

void SpriteFactory::makePressedResetButtonSprite()
{
  auto &buff = sprites->pressedResetButton;
  MinefieldElementArtist::draw2DCellBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);

  const double center = config::INFO_PANEL_BUTTONS_HEIGHT * (0.5 + 0.025);
  FaceArtist::drawFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceAliveEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
};

void SpriteFactory::makeWinnerResetButtonSprite()
{
  auto &buff = sprites->winnerResetButton;
  MinefieldElementArtist::draw3DCellBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);

  const double center = config::INFO_PANEL_BUTTONS_HEIGHT * 0.5;
  FaceArtist::drawFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceShade(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
}

void SpriteFactory::makeLoserResetButtonSprite()
{
  auto &buff = sprites->loserResetButton;
  MinefieldElementArtist::draw3DCellBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);

  const double center = config::INFO_PANEL_BUTTONS_HEIGHT * 0.5;
  FaceArtist::drawFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT, center);
  FaceArtist::drawFaceFrown(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceArtist::drawFaceDeadEye(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
}

void SpriteFactory::makeRaisedConfigButtonSprite()
{
  auto &buff = sprites->raisedConfigButton;
  MinefieldElementArtist::draw3DCellBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);

  const double gearCenter = config::INFO_PANEL_BUTTONS_HEIGHT * 0.5;
  HeaderArtist::drawGear(buff, config::INFO_PANEL_BUTTONS_HEIGHT, gearCenter);
}

void SpriteFactory::makePressedConfigButtonSprite()
{
  auto &buff = sprites->pressedConfigButton;
  MinefieldElementArtist::draw2DCellBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);

  const double gearCenter = config::INFO_PANEL_BUTTONS_HEIGHT * (0.5 + 0.025);
  HeaderArtist::drawGear(buff, config::INFO_PANEL_BUTTONS_HEIGHT, gearCenter);
}

void SpriteFactory::makeEmptyCellSprite()
{
  auto &buff = sprites->empty;
  MinefieldElementArtist::draw2DCellBase(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeHiddenCellSprite()
{
  auto &buff = sprites->hidden;
  MinefieldElementArtist::draw3DCellBase(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeFlaggedCellSprite()
{
  auto &buff = sprites->flag;
  MinefieldElementArtist::draw3DCellBase(buff, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawFlag(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeMineCellSprite()
{
  auto &buff = sprites->mine;
  MinefieldElementArtist::draw2DCellBase(buff, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawMine(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeClickedMineCellSprite()
{
  auto &buff = sprites->clickedMine;
  MinefieldElementArtist::drawClickedMineCell(buff, config::CELL_PIXEL_SIZE);
};

void SpriteFactory::makeMineCellWithRedXSprite()
{
  auto &buff = sprites->redXMine;
  MinefieldElementArtist::draw2DCellBase(buff, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawMine(buff, config::CELL_PIXEL_SIZE);
  FaceArtist::drawX(
      buff,
      config::CELL_PIXEL_SIZE,
      config::RED,
      config::CELL_PIXEL_SIZE / 2,
      config::CELL_PIXEL_SIZE / 2,
      config::CELL_PIXEL_SIZE * 0.7,
      4);
}

void SpriteFactory::makeNumericSprite(std::vector<uint32_t> &buff, const int n, const uint32_t c)
{
  buff = sprites->empty;
  HeaderArtist::drawDigit(
      buff,
      config::CELL_PIXEL_SIZE,
      {config::CELL_PIXEL_SIZE / 2 - NUMERIC_SPRITE_WIDTH / 2,
       config::CELL_PIXEL_SIZE / 2 - NUMERIC_SPRITE_HEIGHT / 2,
       NUMERIC_SPRITE_WIDTH,
       NUMERIC_SPRITE_HEIGHT},
      n,
      c);
};

void SpriteFactory::makeOneSprite()
{
  auto &buff = sprites->one;
  buff = sprites->empty;
  MinefieldElementArtist::drawOne(buff, config::CELL_PIXEL_SIZE);
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