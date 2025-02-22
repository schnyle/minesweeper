#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "config.hpp"

class SpriteFactory
{
public:
  int resetButtonSize = config::INFO_PANEL_BUTTONS_HEIGHT * config::INFO_PANEL_BUTTONS_HEIGHT;
  int cellSpriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE;
  struct Sprites
  {
    std::vector<uint32_t> raisedResetButton;
    std::vector<uint32_t> pressedResetButton;
    std::vector<uint32_t> raisedButton;
    std::vector<uint32_t> pressedButton;
    std::vector<uint32_t> winnerResetButton;
    std::vector<uint32_t> loserResetButton;
    std::vector<uint32_t> empty;
    std::vector<uint32_t> hidden;
    std::vector<uint32_t> flag;
    std::vector<uint32_t> mine;
    std::vector<uint32_t> clickedMine;
    std::vector<uint32_t> redXMine;
    std::vector<uint32_t> zero;
    std::vector<uint32_t> one;
    std::vector<uint32_t> two;
    std::vector<uint32_t> three;
    std::vector<uint32_t> four;
    std::vector<uint32_t> five;
    std::vector<uint32_t> six;
    std::vector<uint32_t> seven;
    std::vector<uint32_t> eight;
    std::map<int, std::vector<uint32_t>> intToSpriteMap;
  };

  SpriteFactory(Sprites *spriteObjs);

  static std::unique_ptr<Sprites> createSprites();
  static void copySprite(
      std::vector<uint32_t> &buff,
      const std::vector<uint32_t> &sprite,
      const int spriteWidth,
      const int x,
      const int y);

private:
  Sprites *sprites;

  int NUMERIC_SPRITE_HEIGHT = 0.6 * config::CELL_PIXEL_SIZE;
  int NUMERIC_SPRITE_WIDTH = NUMERIC_SPRITE_HEIGHT / 2;
  int NUMERIC_SPRITE_PAD = (config::CELL_PIXEL_SIZE - NUMERIC_SPRITE_HEIGHT) / 2;

  void makeRaisedResetButtonSprite();
  void makePressedResetButtonSprite();
  void makeRaisedButtonSprite();
  void makePressedButtonSprite();
  void makeWinnerResetButtonSprite();
  void makeLoserResetButtonSprite();
  void makeEmptyCellSprite();
  void makeHiddenCellSprite();
  void makeFlaggedCellSprite();
  void makeMineCellSprite();
  void makeClickedMineCellSprite();
  void makeMineCellWithRedXSprite();
  void makeNumericSprite(std::vector<uint32_t> &buff, const int n, const uint32_t c);
  void makeOneSprite();

  static int rowColToWindowIndex(const int row, const int col);
  static int rowColToCellIndex(const int row, const int col);
  static std::pair<int, int> rowColToPixelPoint(const int row, const int col);
};