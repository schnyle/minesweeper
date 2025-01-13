#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include <string>

#include "data.hpp"

class Renderer
{
public:
  Renderer();
  ~Renderer();

  void render();

private:
  struct Point
  {
    int x;
    int y;
  };

  const char *APP_NAME{"Minesweeper"};

  int CELL_SIZE = 50;
  int CELL_BORDER_WIDTH_3D = 5;
  int CELL_BORDER_WIDTH_2D = 2; // even int

  uint32_t DARK_GREY = 0x7a7a7b;
  uint32_t GREY = 0xbdbdbd;
  uint32_t LIGHT_GREY = 0xfefffe;

  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;

  void drawCellBase(int row, int col);
  void draw2DEdges(int row, int col);
  void draw3DEdges(int row, int col);
  void drawHiddenCell(int row, int col);
  void drawRevealedCell(int row, int col, int n);
  void overlayImage(int row, int col, uint32_t (&image)[50 * 50], uint32_t transparentHex = 0xffffffff);
  void drawBoard(Cell (&data)[GRID_HEIGHT][GRID_WIDTH]);
  GC createGC();

  Point rowColToPixelPoint(int row, int col);
};