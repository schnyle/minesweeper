#pragma once

#include <Game.hpp>
#include <X11/Xlib.h>
#include <array>
#include <config.hpp>
#include <cstdint>
#include <set>
#include <string>
#include <utility>

#include "data.hpp"

class Renderer
{
public:
  Renderer();
  ~Renderer();

  void run(Game &);

private:
  struct Point
  {
    int x;
    int y;
  };

  struct Images
  {
    char flag[config::IMAGE_SIZE];
    char mine[config::IMAGE_SIZE];
    char one[config::IMAGE_SIZE];
    char two[config::IMAGE_SIZE];
    char three[config::IMAGE_SIZE];
    char four[config::IMAGE_SIZE];
    char five[config::IMAGE_SIZE];
    char six[config::IMAGE_SIZE];
    char seven[config::IMAGE_SIZE];
    char eight[config::IMAGE_SIZE];

  } images;

  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;

  void drawCellBase(const int row, const int col) const;
  void drawMineCellBase(const int row, const int col) const;
  void draw2DEdges(const int row, const int col) const;
  void draw3DEdges(const int row, const int col) const;
  void drawHiddenCell(const int row, const int col) const;
  void drawRevealedCell(const int row, const int col) const;
  void drawAdjacentMinesNum(const int row, const int col, const int n) const;
  void drawRevealedMineCell(const int row, const int col) const;
  void drawBoard(const Game::Minefield &minefield) const;
  void overlayImage(const int row, const int col, const char *image, const uint32_t transparentHex = 0xffffffff) const;

  void loadBinaryFile(const std::string &filepath, char (&dest)[config::IMAGE_SIZE]);
  void loadImageData();
  GC createGC();
  Point rowColToPixelPoint(const int row, const int col) const;
};