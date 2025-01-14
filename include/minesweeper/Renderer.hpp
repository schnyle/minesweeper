#pragma once

#include <X11/Xlib.h>
#include <array>
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

  const char *APP_NAME{"Minesweeper"};

  int CELL_SIZE = 50;
  int CELL_BORDER_WIDTH_3D = 5;
  int CELL_BORDER_WIDTH_2D = 2; // even int

  uint32_t DARK_GREY = 0x7a7a7b;
  uint32_t GREY = 0xbdbdbd;
  uint32_t LIGHT_GREY = 0xfefffe;

  std::array<Cell, GRID_HEIGHT * GRID_WIDTH> data;

  static constexpr size_t IMAGE_SIZE = 50 * 50 * 4;
  struct Images
  {
    char flag[IMAGE_SIZE];
    char mine[IMAGE_SIZE];
    char one[IMAGE_SIZE];
    char two[IMAGE_SIZE];
    char three[IMAGE_SIZE];
    char four[IMAGE_SIZE];
    char five[IMAGE_SIZE];
    char six[IMAGE_SIZE];
    char seven[IMAGE_SIZE];
    char eight[IMAGE_SIZE];

  } images;

  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;

  // clang-format off
  const std::array<std::pair<int, int>, 8> ADJACENCY_OFFSETS = {{
    { 1, -1}, { 1, 0}, { 1, 1},
    { 0, -1},          { 0, 1},
    {-1, -1}, {-1, 0}, {-1, 1}
  }};
  // clang-format on

  void run();
  void revealAdjacentCells(int row, int col);
  void floodFillEmptyCells(int row, int col);
  void floodFillEmptyCellsRecursive(int row, int col, std::set<std::pair<int, int>> &visited);

  void drawCellBase(int row, int col);
  void draw2DEdges(int row, int col);
  void draw3DEdges(int row, int col);
  void drawHiddenCell(int row, int col);
  void drawRevealedCell(int row, int col);
  void drawAdjacentMinesNum(int row, int col, int n);
  void overlayImage(int row, int col, const char *image, uint32_t transparentHex = 0xffffffff);
  void drawBoard();

  void loadBinaryFile(const std::string &filepath, char (&dest)[IMAGE_SIZE]);
  void loadImageData();
  GC createGC();
  int rowColToIndex(const int row, const int col) const;
  Point rowColToPixelPoint(const int row, const int col) const;
};