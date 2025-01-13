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
  const char *APP_NAME{"Minesweeper"};

  unsigned int CELL_SIZE = 50;
  unsigned int CELL_BORDER_WIDTH_3D = 5;
  unsigned int CELL_BORDER_WIDTH_2D = 2; // even int

  uint32_t DARK_GREY = 0x7a7a7b;
  uint32_t GREY = 0xbdbdbd;
  uint32_t LIGHT_GREY = 0xfefffe;

  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;

  void drawUnrevealedCell(int x, int y);
  void drawRevealedCell(int x, int y, int n);
  void drawFlaggedCell(int x, int y);
  void drawBoard(Cell (&data)[GRID_HEIGHT][GRID_WIDTH]);
  GC createGC();
};