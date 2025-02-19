#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <utility>

#include "config.hpp"

class SpriteFactory
{
public:
  static constexpr int resetButtonSize = config::INFO_PANEL_BUTTONS_HEIGHT * config::INFO_PANEL_BUTTONS_HEIGHT;
  static constexpr int cellSpriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE;
  struct Sprites
  {
    uint32_t raisedResetButton[resetButtonSize];
    uint32_t pressedResetButton[resetButtonSize];
    uint32_t raisedButton[resetButtonSize];
    uint32_t pressedButton[resetButtonSize];
    uint32_t winnerResetButton[resetButtonSize];
    uint32_t loserResetButton[resetButtonSize];
    uint32_t empty[cellSpriteSize];
    uint32_t hidden[cellSpriteSize];
    uint32_t flag[cellSpriteSize];
    uint32_t mine[cellSpriteSize];
    uint32_t clickedMine[cellSpriteSize];
    uint32_t redXMine[cellSpriteSize];
    uint32_t zero[cellSpriteSize];
    uint32_t one[cellSpriteSize];
    uint32_t two[cellSpriteSize];
    uint32_t three[cellSpriteSize];
    uint32_t four[cellSpriteSize];
    uint32_t five[cellSpriteSize];
    uint32_t six[cellSpriteSize];
    uint32_t seven[cellSpriteSize];
    uint32_t eight[cellSpriteSize];
    std::map<int, uint32_t *> intToSpriteMap;
  };

  SpriteFactory(Sprites *spriteObjs);

  static std::unique_ptr<Sprites> createSprites();
  static void copySprite(uint32_t *buff, const uint32_t *sprite, const int spriteWidth, const int x, const int y);
  static void
  copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t *sprite, const int spriteWidth, const int x, const int y);

private:
  Sprites *sprites;

  static const int NUMERIC_SPRITE_HEIGHT = 0.6 * config::CELL_PIXEL_SIZE;
  static const int NUMERIC_SPRITE_WIDTH = NUMERIC_SPRITE_HEIGHT / 2;
  static const int NUMERIC_SPRITE_PAD = (config::CELL_PIXEL_SIZE - NUMERIC_SPRITE_HEIGHT) / 2;

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
  void makeNumericSprite(uint32_t *buff, const int n, const uint32_t c);
  void makeOneSprite();

  static int rowColToWindowIndex(const int row, const int col);
  static int rowColToCellIndex(const int row, const int col);
  static std::pair<int, int> rowColToPixelPoint(const int row, const int col);
};