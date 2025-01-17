#pragma once

#include <Game.hpp>
#include <SpriteFactory.hpp>
#include <X11/Xlib.h>
#include <config.hpp>
#include <cstdint>
#include <map>
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

  std::unique_ptr<SpriteFactory::Sprites> sprites;

  void renderFrame();
  void updateGameState(Game &, XEvent &);
  void updateBackBuffer(Game &);

  void initializeGC();
  void initializeBuffers();

  // move
  void makeInterface();
  void makeInfoPanel();

  // move
  void copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t *sprite, const int row, const int col);
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