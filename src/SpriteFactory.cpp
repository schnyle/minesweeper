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

  HeaderArtist::drawRaisedResetButtonSprite(sprites->raisedResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawPressedResetButtonSprite(sprites->pressedResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawWinnerResetButtonSprite(sprites->winnerResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawLoserResetButtonSprite(sprites->loserResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawRaisedConfigButtonSprite(sprites->raisedConfigButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawPressedConfigButtonSprite(sprites->pressedConfigButton, config::INFO_PANEL_BUTTONS_HEIGHT);

  MinefieldElementArtist::draw2DCellBase(sprites->empty, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::draw3DCellBase(sprites->hidden, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawFlaggedCellSprite(sprites->flag, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawMineCellSprite(sprites->mine, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawClickedMineCell(sprites->clickedMine, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawMineCellRedXSprite(sprites->redXMine, config::CELL_PIXEL_SIZE);

  MinefieldElementArtist::drawNumericSprite(sprites->one, config::CELL_PIXEL_SIZE, 1, config::BLUE);
  MinefieldElementArtist::drawNumericSprite(sprites->two, config::CELL_PIXEL_SIZE, 2, config::GREEN);
  MinefieldElementArtist::drawNumericSprite(sprites->three, config::CELL_PIXEL_SIZE, 3, config::RED);
  MinefieldElementArtist::drawNumericSprite(sprites->four, config::CELL_PIXEL_SIZE, 4, config::DARK_BLUE);
  MinefieldElementArtist::drawNumericSprite(sprites->five, config::CELL_PIXEL_SIZE, 5, config::DARK_RED);
  MinefieldElementArtist::drawNumericSprite(sprites->six, config::CELL_PIXEL_SIZE, 6, config::TURQUOISE);
  MinefieldElementArtist::drawNumericSprite(sprites->seven, config::CELL_PIXEL_SIZE, 7, config::PURPLE);
  MinefieldElementArtist::drawNumericSprite(sprites->eight, config::CELL_PIXEL_SIZE, 7, config::DARK_GREY);

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
    const auto destinationRow = buff.begin() + (row + y) * config::GAME_WINDOW_PIXEL_WIDTH + x;
    std::copy(sourceRow, sourceRowEnd, destinationRow);
  }
}
