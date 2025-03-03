#include <HeaderArtist.hpp>
#include <MinefieldArtist.hpp>
#include <Sprites.hpp>
#include <algorithm>
#include <config.hpp>
#include <cstdint>
#include <memory>
#include <vector>

Sprites::Sprites()
{
  allocateMemory();
  drawSprites();
  createIntToSpriteMap();
}

Sprites &Sprites::getInstance()
{
  static Sprites instance;
  return instance;
}

const Sprites::SpriteData *Sprites::get() { return data.get(); };

void Sprites::copy(
    const std::vector<uint32_t> &source,
    std::vector<uint32_t> &target,
    const int width,
    const int x,
    const int y)
{
  // assumes square buffers
  for (int row = 0; row < width; ++row)
  {
    const auto first = source.begin() + row * width;
    const auto result = target.begin() + (row + y) * config::getSettings().getGameWindowWidth() + x;
    std::copy_n(first, width, result);
  }
};

void Sprites::allocateMemory()
{
  int resetButtonSize = config::INFO_PANEL_BUTTONS_HEIGHT * config::INFO_PANEL_BUTTONS_HEIGHT;
  int cellSpriteSize = config::getSettings().getCellPixelSize() * config::getSettings().getCellPixelSize();

  data->raisedResetButton.resize(resetButtonSize);
  data->pressedResetButton.resize(resetButtonSize);
  data->winnerResetButton.resize(resetButtonSize);
  data->loserResetButton.resize(resetButtonSize);
  data->raisedConfigButton.resize(resetButtonSize);
  data->pressedConfigButton.resize(resetButtonSize);
  data->empty.resize(cellSpriteSize);
  data->hidden.resize(cellSpriteSize);
  data->flag.resize(cellSpriteSize);
  data->mine.resize(cellSpriteSize);
  data->clickedMine.resize(cellSpriteSize);
  data->redXMine.resize(cellSpriteSize);
  data->zero.resize(cellSpriteSize);
  data->one.resize(cellSpriteSize);
  data->two.resize(cellSpriteSize);
  data->three.resize(cellSpriteSize);
  data->four.resize(cellSpriteSize);
  data->five.resize(cellSpriteSize);
  data->six.resize(cellSpriteSize);
  data->seven.resize(cellSpriteSize);
  data->eight.resize(cellSpriteSize);
};

void Sprites::drawSprites()
{
  HeaderArtist::drawRaisedResetButtonSprite(data->raisedResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawPressedResetButtonSprite(data->pressedResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawWinnerResetButtonSprite(data->winnerResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawLoserResetButtonSprite(data->loserResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawRaisedConfigButtonSprite(data->raisedConfigButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawPressedConfigButtonSprite(data->pressedConfigButton, config::INFO_PANEL_BUTTONS_HEIGHT);

  MinefieldArtist::drawEmptyCellSprite(data->empty, config::getSettings().getCellPixelSize());
  MinefieldArtist::drawHiddenCellSprite(data->hidden, config::getSettings().getCellPixelSize());
  MinefieldArtist::drawFlaggedCellSprite(data->flag, config::getSettings().getCellPixelSize());
  MinefieldArtist::drawMineCellSprite(data->mine, config::getSettings().getCellPixelSize());
  MinefieldArtist::drawClickedMineCellSprite(data->clickedMine, config::getSettings().getCellPixelSize());
  MinefieldArtist::drawMineCellRedXSprite(data->redXMine, config::getSettings().getCellPixelSize());

  MinefieldArtist::drawNumericSprite(data->one, config::getSettings().getCellPixelSize(), 1, config::Colors::BLUE);
  MinefieldArtist::drawNumericSprite(data->two, config::getSettings().getCellPixelSize(), 2, config::Colors::GREEN);
  MinefieldArtist::drawNumericSprite(data->three, config::getSettings().getCellPixelSize(), 3, config::Colors::RED);
  MinefieldArtist::drawNumericSprite(
      data->four, config::getSettings().getCellPixelSize(), 4, config::Colors::DARK_BLUE);
  MinefieldArtist::drawNumericSprite(data->five, config::getSettings().getCellPixelSize(), 5, config::Colors::DARK_RED);
  MinefieldArtist::drawNumericSprite(data->six, config::getSettings().getCellPixelSize(), 6, config::Colors::TURQUOISE);
  MinefieldArtist::drawNumericSprite(data->seven, config::getSettings().getCellPixelSize(), 7, config::Colors::PURPLE);
  MinefieldArtist::drawNumericSprite(
      data->eight, config::getSettings().getCellPixelSize(), 8, config::Colors::DARK_GREY);
};

void Sprites::createIntToSpriteMap()
{
  data->intToSpriteMap = {
      {0, data->empty},
      {1, data->one},
      {2, data->two},
      {3, data->three},
      {4, data->four},
      {5, data->five},
      {6, data->six},
      {7, data->seven},
      {8, data->eight},
  };
};