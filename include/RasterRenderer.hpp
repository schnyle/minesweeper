#pragma once

#include <Game.hpp>
#include <X11/Xlib.h>
#include <config.hpp>
#include <cstdint>
#include <memory>
#include <utility>

class RasterRenderer
{
public:
  RasterRenderer();
  ~RasterRenderer();

  void run(Game &);

private:
  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;
  XImage *image;
  std::unique_ptr<uint32_t[]> frontBuffer;
  std::unique_ptr<uint32_t[]> backBuffer;

  struct Sprites
  {
    static constexpr int spriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE * 4;
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
  } sprites;

  void renderFrame();
  void updateGameState(Game &, XEvent &);
  void updateBackBuffer(Game &);

  void initializeGC();
  void initializeBuffers();
  void initializeSprites();

  void makeInterface();
  void makeHiddenCellSprite();
  void makeFlaggedCellSprite();

  void copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t (&sprite)[], const int row, const int col);
  void buffInsertRectangle(
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