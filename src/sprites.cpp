#include <HeaderArtist.hpp>
#include <MinefieldElementArtist.hpp>
#include <config.hpp>
#include <cstdint>
#include <memory>
#include <sprites.hpp>
#include <vector>

namespace sprites
{
std::unique_ptr<SpriteData> data = std::make_unique<SpriteData>();
void init()
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

  HeaderArtist::drawRaisedResetButtonSprite(data->raisedResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawPressedResetButtonSprite(data->pressedResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawWinnerResetButtonSprite(data->winnerResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawLoserResetButtonSprite(data->loserResetButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawRaisedConfigButtonSprite(data->raisedConfigButton, config::INFO_PANEL_BUTTONS_HEIGHT);
  HeaderArtist::drawPressedConfigButtonSprite(data->pressedConfigButton, config::INFO_PANEL_BUTTONS_HEIGHT);

  MinefieldElementArtist::draw2DCellBase(data->empty, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::draw3DCellBase(data->hidden, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawFlaggedCellSprite(data->flag, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawMineCellSprite(data->mine, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawClickedMineCell(data->clickedMine, config::CELL_PIXEL_SIZE);
  MinefieldElementArtist::drawMineCellRedXSprite(data->redXMine, config::CELL_PIXEL_SIZE);

  MinefieldElementArtist::drawNumericSprite(data->one, config::CELL_PIXEL_SIZE, 1, config::BLUE);
  MinefieldElementArtist::drawNumericSprite(data->two, config::CELL_PIXEL_SIZE, 2, config::GREEN);
  MinefieldElementArtist::drawNumericSprite(data->three, config::CELL_PIXEL_SIZE, 3, config::RED);
  MinefieldElementArtist::drawNumericSprite(data->four, config::CELL_PIXEL_SIZE, 4, config::DARK_BLUE);
  MinefieldElementArtist::drawNumericSprite(data->five, config::CELL_PIXEL_SIZE, 5, config::DARK_RED);
  MinefieldElementArtist::drawNumericSprite(data->six, config::CELL_PIXEL_SIZE, 6, config::TURQUOISE);
  MinefieldElementArtist::drawNumericSprite(data->seven, config::CELL_PIXEL_SIZE, 7, config::PURPLE);
  MinefieldElementArtist::drawNumericSprite(data->eight, config::CELL_PIXEL_SIZE, 8, config::DARK_GREY);

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
}

const SpriteData *get() { return data.get(); };

void copy(const std::vector<uint32_t> &source, std::vector<uint32_t> &target, const int width, const int x, const int y)
{
  for (int row = 0; row < width; ++row)
  {
    const auto first = source.begin() + row * width;
    const auto last = source.begin() + (row * width + width);
    const auto result = target.begin() + (row + y) * config::GAME_WINDOW_PIXEL_WIDTH + x;
    std::copy(first, last, result);
  }
};

} // namespace sprites
