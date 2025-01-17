#pragma once

#include <config.hpp>
#include <cstdint>
#include <map>
#include <memory>

class SpriteFactory
{
public:
  static constexpr int spriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE * 4;

  struct Sprites
  {
    uint32_t empty[spriteSize];
    uint32_t hidden[spriteSize];
    uint32_t flag[spriteSize];
    uint32_t mine[spriteSize];
    uint32_t zero[spriteSize];
    uint32_t one[spriteSize];
    uint32_t two[spriteSize];
    uint32_t three[spriteSize];
    uint32_t four[spriteSize];
    uint32_t five[spriteSize];
    uint32_t six[spriteSize];
    uint32_t seven[spriteSize];
    uint32_t eight[spriteSize];
    std::map<int, uint32_t *> intToSpriteMap;
  };

  SpriteFactory(Sprites *spriteObjs);

  static std::unique_ptr<Sprites> createSprites();

private:
  Sprites *sprites;

  static const int NUMERIC_SPRITE_SIZE = 0.6 * config::CELL_PIXEL_SIZE;
  static const int NUMERIC_SPRITE_PAD = (config::CELL_PIXEL_SIZE - NUMERIC_SPRITE_SIZE) / 2;
  void makeEmptyCellSprite();
  void makeHiddenCellSprite();
  void makeFlaggedCellSprite();
  void makeMineCellSprite();
  void makeOneSprite();
  void makeTwoSprite();
  void makeThreeSprite();
  void makeFourSprite();
  void makeFiveSprite();
  void makeSixSprite();
  void makeSevenSprite();
  void makeEightSprite();

  void copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t *sprite, const int row, const int col);
  void copyNumericSprite(uint32_t *dest, uint32_t *source);

  void buffInsertRectangle(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t c);
  void buffInsert2DBorder(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t c);
  void buffInsert3DBorder(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  void buffInsert3DCorner(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);

  int rowColToWindowIndex(const int row, const int col) const;
  int rowColToCellIndex(const int row, const int col) const;
  std::pair<int, int> rowColToPixelPoint(const int row, const int col) const;
};