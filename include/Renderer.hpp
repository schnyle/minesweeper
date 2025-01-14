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

  void render();

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

  void run();

  void drawCellBase(int row, int col);
  void draw2DEdges(int row, int col);
  void draw3DEdges(int row, int col);
  void drawHiddenCell(int row, int col);
  void drawRevealedCell(int row, int col);
  void drawAdjacentMinesNum(int row, int col, int n);
  void overlayImage(int row, int col, const char *image, uint32_t transparentHex = 0xffffffff);
  void drawBoard(const Game::Minefield &minefield);

  void loadBinaryFile(const std::string &filepath, char (&dest)[config::IMAGE_SIZE]);
  void loadImageData();
  GC createGC();
  Point rowColToPixelPoint(const int row, const int col) const;
};