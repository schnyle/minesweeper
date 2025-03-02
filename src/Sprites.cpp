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
    const auto result = target.begin() + (row + y) * config::GAME_WINDOW_PIXEL_WIDTH + x;
    std::copy_n(first, width, result);
  }
};

void Sprites::allocateMemory()
{
  int resetButtonSize = config::INFO_PANEL_BUTTONS_HEIGHT * config::INFO_PANEL_BUTTONS_HEIGHT;
  int cellSpriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE;

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

  MinefieldArtist::drawEmptyCellSprite(data->empty, config::CELL_PIXEL_SIZE);
  MinefieldArtist::drawHiddenCellSprite(data->hidden, config::CELL_PIXEL_SIZE);
  MinefieldArtist::drawFlaggedCellSprite(data->flag, config::CELL_PIXEL_SIZE);
  MinefieldArtist::drawMineCellSprite(data->mine, config::CELL_PIXEL_SIZE);
  MinefieldArtist::drawClickedMineCellSprite(data->clickedMine, config::CELL_PIXEL_SIZE);
  MinefieldArtist::drawMineCellRedXSprite(data->redXMine, config::CELL_PIXEL_SIZE);

  MinefieldArtist::drawNumericSprite(data->one, config::CELL_PIXEL_SIZE, 1, config::BLUE);
  MinefieldArtist::drawNumericSprite(data->two, config::CELL_PIXEL_SIZE, 2, config::GREEN);
  MinefieldArtist::drawNumericSprite(data->three, config::CELL_PIXEL_SIZE, 3, config::RED);
  MinefieldArtist::drawNumericSprite(data->four, config::CELL_PIXEL_SIZE, 4, config::DARK_BLUE);
  MinefieldArtist::drawNumericSprite(data->five, config::CELL_PIXEL_SIZE, 5, config::DARK_RED);
  MinefieldArtist::drawNumericSprite(data->six, config::CELL_PIXEL_SIZE, 6, config::TURQUOISE);
  MinefieldArtist::drawNumericSprite(data->seven, config::CELL_PIXEL_SIZE, 7, config::PURPLE);
  MinefieldArtist::drawNumericSprite(data->eight, config::CELL_PIXEL_SIZE, 8, config::DARK_GREY);
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