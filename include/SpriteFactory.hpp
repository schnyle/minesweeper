#pragma once

#include <config.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>

class SpriteFactory
{
public:
  static constexpr int resetButtonSize = config::INFO_PANEL_BUTTONS_HEIGHT * config::INFO_PANEL_BUTTONS_HEIGHT;
  static constexpr int cellSpriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE;
  struct Sprites
  {
    uint32_t raisedButton[resetButtonSize];
    uint32_t pressedButton[resetButtonSize];
    uint32_t empty[cellSpriteSize];
    uint32_t hidden[cellSpriteSize];
    uint32_t flag[cellSpriteSize];
    uint32_t mine[cellSpriteSize];
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
  static void buffInsertInterface(uint32_t *buff, const int buffWidth, const int buffSize);
  static void buffInsertRemainingFlags(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const int n);
  static void
  buffInsertTimer(uint32_t *buff, const int buffWidth, const int x, const int y, const int w, const int h, const int t);
  static void
  copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t *sprite, const int spriteWidth, const int x, const int y);

private:
  Sprites *sprites;

  static const int NUMERIC_SPRITE_HEIGHT = 0.6 * config::CELL_PIXEL_SIZE;
  static const int NUMERIC_SPRITE_WIDTH = NUMERIC_SPRITE_HEIGHT / 2;
  static const int NUMERIC_SPRITE_PAD = (config::CELL_PIXEL_SIZE - NUMERIC_SPRITE_HEIGHT) / 2;
  void makeRaisedButtonSprite();
  void makePressedButtonSprite();
  void makeEmptyCellSprite();
  void makeHiddenCellSprite();
  void makeFlaggedCellSprite();
  void makeMineCellSprite();
  void makeNumericSprite(uint32_t *buff, const int n, const uint32_t c);
  void makeOneSprite();

  static void buffInsertRectangle(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t c);
  static void buffInsert2DBorder(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t c);
  static void buffInsert3DBorder(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void buffInsert3DCorner(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void buffInsertDigit(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const int n,
      const int c);

  struct DigitSegments
  {
    bool topMiddle;
    bool topLeft;
    bool topRight;
    bool middleMiddle;
    bool bottomMiddle;
    bool bottomLeft;
    bool bottomRight;
  };
  static DigitSegments intToDigitSegments(const int n);

  static int rowColToWindowIndex(const int row, const int col);
  static int rowColToCellIndex(const int row, const int col);
  static std::pair<int, int> rowColToPixelPoint(const int row, const int col);
};