#include <X11/Xlib.h>
#include <fstream>
#include <iostream>
#include <minesweeper/Renderer.hpp>
#include <minesweeper/data.hpp>
#include <set>
#include <stdexcept>
#include <utility>

Renderer::Renderer()
{
  display = XOpenDisplay(nullptr);
  if (!display)
  {
    throw std::runtime_error("Unable to open X display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  visual = DefaultVisual(display, screen);

  XSetWindowAttributes xwa = {};
  xwa.background_pixel = WhitePixel(display, screen);
  xwa.border_pixel = BlackPixel(display, screen);
  xwa.event_mask = ExposureMask | Button1MotionMask | ButtonPressMask | ButtonReleaseMask;
  window = XCreateWindow(
      display,
      root,
      0,
      0,
      1920,
      1080,
      1,
      DefaultDepth(display, screen),
      InputOutput,
      visual,
      CWBackPixel | CWEventMask | CWBorderPixel,
      &xwa

  );
  gc = createGC();
  loadImageData();

  data = generateData();
}

Renderer::~Renderer() { XCloseDisplay(display); }

void Renderer::render()
{
  XStoreName(display, window, APP_NAME);
  XMapWindow(display, window);

  run();
}

void Renderer::run()
{
  bool first = true;
  XEvent event;

  while (true)
  {
    XNextEvent(display, &event);

    switch (event.type)
    {
    case Expose:
      drawBoard();
      break;
    case ButtonPress:
      const int row = event.xbutton.y / CELL_SIZE;
      const int col = event.xbutton.x / CELL_SIZE;
      const int index = row * GRID_WIDTH + col;

      if (event.xbutton.button == Button1 && data[index].isHidden)
      {
        if (first)
        {
          first = false;
          while (data[index].nAdjacentMines != 0 || data[index].isMine)
          {
            data = generateData();
          }
        }

        if (data[index].isFlagged)
        {
          break;
        }

        data[index].isHidden = false;
        if (data[index].nAdjacentMines == 0)
        {
          floodFillEmptyCells(row, col);
        }
      }
      else if (event.xbutton.button == Button3)
      {
        if (data[index].isHidden)
        {
          data[index].isFlagged = !data[index].isFlagged;
        }
      }
      else if (event.xbutton.button == Button2)
      {
        revealAdjacentCells(row, col);
      }
      break;
    }

    drawBoard();
  }
}

void Renderer::revealAdjacentCells(int row, int col)
{
  unsigned int nFlags = 0;
  std::set<std::pair<int, int>> hidden;

  for (const auto &[dRow, dCol] : ADJACENCY_OFFSETS)
  {
    const int currentRow = row + dRow;
    const int currentCol = col + dCol;
    if (currentRow < 0 || currentCol < 0 || currentRow >= GRID_HEIGHT || currentCol >= GRID_WIDTH)
    {
      continue;
    }

    const auto currentIndex = rowColToIndex(currentRow, currentCol);
    if (data[currentIndex].isFlagged)
    {
      ++nFlags;
    }
    else if (data[currentIndex].isHidden)
    {
      hidden.insert({currentRow, currentCol});
    }
  }

  if (nFlags == data[rowColToIndex(row, col)].nAdjacentMines)
  {
    for (const auto &[currentRow, currentCol] : hidden)
    {
      const auto currentIndex = rowColToIndex(currentRow, currentCol);
      data[currentIndex].isHidden = false;
      if (data[currentIndex].nAdjacentMines == 0)
      {
        floodFillEmptyCells(currentRow, currentCol);
      }
    }
  }
}

void Renderer::floodFillEmptyCells(int row, int col)
{
  std::set<std::pair<int, int>> visited;
  floodFillEmptyCellsRecursive(row, col, visited);
}

void Renderer::floodFillEmptyCellsRecursive(int row, int col, std::set<std::pair<int, int>> &visited)
{
  visited.insert({row, col});

  for (const auto &[dRow, dCol] : ADJACENCY_OFFSETS)
  {
    const int newRow = row + dRow;
    const int newCol = col + dCol;
    if (newRow < 0 || newCol < 0 || newRow >= GRID_HEIGHT || newCol >= GRID_WIDTH)
    {
      continue;
    }

    const std::pair<int, int> p{newRow, newCol};
    if (visited.find(p) != visited.end())
    {
      continue;
    }
    else
    {
      visited.insert(p);
    }

    const int index = p.first * GRID_WIDTH + p.second;
    if (!data[index].isMine)
    {
      data[index].isHidden = false;
      if (data[index].nAdjacentMines == 0)
      {
        floodFillEmptyCellsRecursive(newRow, newCol, visited);
      }
    }
  }
}

void Renderer::drawCellBase(int row, int col)
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, GREY);
  XFillRectangle(display, window, gc, p.x, p.y, CELL_SIZE, CELL_SIZE);
}

void Renderer::draw2DEdges(int row, int col)
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, p.x - 1, p.y - 1, CELL_SIZE + 1, CELL_BORDER_WIDTH_2D);
  XFillRectangle(display, window, gc, p.x - 1, p.y - 1, CELL_BORDER_WIDTH_2D, CELL_SIZE + 1);
}

void Renderer::draw3DEdges(int row, int col)
{
  Point pMin = rowColToPixelPoint(row, col);
  Point pMax{pMin.x + CELL_SIZE - 1, pMin.y + CELL_SIZE - 1};

  // top/left edges
  XSetForeground(display, gc, LIGHT_GREY);
  XFillRectangle(display, window, gc, pMin.x, pMin.y, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pMin.x, pMin.y, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  // bottom/right edges
  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, pMin.x, pMax.y - CELL_BORDER_WIDTH_3D + 1, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pMax.x - CELL_BORDER_WIDTH_3D + 1, pMin.y, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  static int SHAPE = Complex;
  static int MODE = CoordModeOrigin;

  // bottom left corner
  XPoint blPoints[3] = {
      {static_cast<short>(pMin.x), static_cast<short>(pMax.y - CELL_BORDER_WIDTH_3D)},
      {static_cast<short>(pMin.x), static_cast<short>(pMax.y)},
      {static_cast<short>(pMin.x + CELL_BORDER_WIDTH_3D), static_cast<short>(pMax.y - CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, blPoints, 3, SHAPE, MODE);

  // top right corner
  XPoint trPoints[3] = {
      {static_cast<short>(pMax.x - CELL_BORDER_WIDTH_3D), static_cast<short>(pMin.y)},
      {static_cast<short>(pMax.x), static_cast<short>(pMin.y)},
      {static_cast<short>(pMax.x - CELL_BORDER_WIDTH_3D), static_cast<short>(pMin.y + CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, trPoints, 3, SHAPE, MODE);
}

void Renderer::drawHiddenCell(int row, int col)
{
  drawCellBase(row, col);
  draw3DEdges(row, col);
}

void Renderer::drawRevealedCell(int row, int col)
{
  drawCellBase(row, col);
  draw2DEdges(row, col);
}

void Renderer::drawAdjacentMinesNum(int row, int col, int n)
{

  switch (n)
  {
  case 0:
    return;
  case 1:
    overlayImage(row, col, images.one);
    break;
  case 2:
    overlayImage(row, col, images.two);
    break;
  case 3:
    overlayImage(row, col, images.three);
    break;
  case 4:
    overlayImage(row, col, images.four);
    break;
  case 5:
    overlayImage(row, col, images.five);
    break;
  case 6:
    overlayImage(row, col, images.six);
    break;
  case 7:
    overlayImage(row, col, images.seven);
    break;
  case 8:
    overlayImage(row, col, images.eight);
    break;
  default:
    overlayImage(row, col, images.one);
    return;
  }
}

void Renderer::overlayImage(int row, int col, const char *image, uint32_t transparentHex)
{
  Point p = rowColToPixelPoint(row, col);

  for (int i = 0; i < CELL_SIZE; ++i)
  {
    for (int j = 0; j < CELL_SIZE; ++j)
    {
      const char *pixelPtr = image + (i * CELL_SIZE + j) * 4;
      uint32_t pixel = *reinterpret_cast<const uint32_t *>(pixelPtr);
      if (pixel != transparentHex)
      {
        XSetForeground(display, gc, pixel);
        XDrawPoint(display, window, gc, p.x + j, p.y + i);
      }
    }
  }
}

void Renderer::drawBoard()
{
  for (size_t row = 0; row < GRID_HEIGHT; ++row)
  {
    for (size_t col = 0; col < GRID_WIDTH; ++col)
    {
      const size_t index = row * GRID_WIDTH + col;

      if (data[index].isHidden)
      {
        drawHiddenCell(row, col);

        if (data[index].isFlagged)
        {
          overlayImage(row, col, images.flag);
        }
      }
      else
      {
        drawRevealedCell(row, col);

        if (data[index].isMine)
        {
          overlayImage(row, col, images.mine);
        }
        else
        {
          drawAdjacentMinesNum(row, col, data[index].nAdjacentMines);
        }
      }
    }
  }
}

void Renderer::loadBinaryFile(const std::string &filepath, char (&dest)[IMAGE_SIZE])
{
  std::ifstream file(filepath, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  file.read(dest, IMAGE_SIZE);
  if (file.gcount() != IMAGE_SIZE)
  {
    throw std::runtime_error("failed to read complete image data for " + filepath);
  }
};

void Renderer::loadImageData()
{
  loadBinaryFile("assets/flag.bin", images.flag);
  loadBinaryFile("assets/mine.bin", images.mine);
  loadBinaryFile("assets/one.bin", images.one);
  loadBinaryFile("assets/two.bin", images.two);
  loadBinaryFile("assets/three.bin", images.three);
  loadBinaryFile("assets/four.bin", images.four);
  loadBinaryFile("assets/five.bin", images.five);
  loadBinaryFile("assets/six.bin", images.six);
  loadBinaryFile("assets/seven.bin", images.seven);
  loadBinaryFile("assets/eight.bin", images.eight);
}

GC Renderer::createGC()
{
  GC gc;
  XGCValues xgcv;

  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  xgcv.join_style = JoinMiter;
  xgcv.fill_style = FillSolid;
  xgcv.foreground = BlackPixel(display, screen);
  xgcv.background = WhitePixel(display, screen);

  unsigned long valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCLineWidth | GCLineWidth |
                            GCCapStyle | GCJoinStyle;

  gc = XCreateGC(display, root, valueMask, &xgcv);
  return gc;
}

int Renderer::rowColToIndex(const int row, const int col) const { return row * GRID_WIDTH + col; }

Renderer::Point Renderer::rowColToPixelPoint(const int row, const int col) const
{
  return {col * CELL_SIZE, row * CELL_SIZE};
}
