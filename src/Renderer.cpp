#include <X11/Xlib.h>
#include <minesweeper/Renderer.hpp>
#include <minesweeper/data.hpp>
#include <stdexcept>

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
  window = XCreateSimpleWindow(
      display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
  gc = createGC();
}

Renderer::~Renderer() { XCloseDisplay(display); }

void Renderer::render()
{
  XStoreName(display, window, APP_NAME);
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  XEvent event;
  while (true)
  {
    drawBoard(data);
    XNextEvent(display, &event);
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

void Renderer::drawUnrevealedCell(int row, int col)
{
  drawCellBase(row, col);
  draw3DEdges(row, col);
}

void Renderer::drawRevealedCell(int row, int col, int n)
{
  int pixelXMin = col * CELL_SIZE;
  int pixelYMin = row * CELL_SIZE;

  drawCellBase(row, col);
  draw2DEdges(row, col);

  // number of adjacent mines
  if (n)
  {
    XImage *one = XCreateImage(
        display,
        visual,
        DefaultDepth(display, screen),
        ZPixmap,
        0,
        (char *)onePixel,
        CELL_SIZE,
        CELL_SIZE,
        32,
        CELL_SIZE * sizeof(uint32_t));
    XPutImage(display, window, gc, one, 0, 0, pixelXMin, pixelYMin, CELL_SIZE, CELL_SIZE);
  }
}

void Renderer::drawFlaggedCell(int row, int col)
{
  short pixelXMin = col * CELL_SIZE;
  short pixelYMin = row * CELL_SIZE;

  drawCellBase(row, col);
  draw3DEdges(row, col);

  // XImage *flag = XCreateImage(
  //     display,
  //     visual,
  //     DefaultDepth(display, screen),
  //     ZPixmap,
  //     0,
  //     (char *)flagPixel,
  //     CELL_SIZE,
  //     CELL_SIZE,
  //     32,
  //     CELL_SIZE * sizeof(uint32_t));

  // For each pixel in your flag image
  for (int i = 0; i < CELL_SIZE; i++)
  {
    for (int j = 0; j < CELL_SIZE; j++)
    {
      uint32_t pixel = flagPixel[i * CELL_SIZE + j];
      if (pixel != 0xFFFFFFFF)
      {
        XSetForeground(display, gc, pixel);
        XDrawPoint(display, window, gc, pixelXMin + j, pixelYMin + i);
      }
    }
  }
}

void Renderer::drawBoard(Cell (&data)[GRID_HEIGHT][GRID_WIDTH])
{
  for (size_t row = 0; row < GRID_HEIGHT; ++row)
  {
    for (size_t col = 0; col < GRID_WIDTH; ++col)
    {
      if (!data[row][col].isRevealed)
      {
        if (data[row][col].isFlagged)
        {
          drawFlaggedCell(row, col);
        }
        else
        {
          drawUnrevealedCell(row, col);
        }
      }
      else
      {
        drawRevealedCell(row, col, data[row][col].nAdjacentMines);
      }
    }
  }
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

Renderer::Point Renderer::rowColToPixelPoint(int row, int col) { return {col * CELL_SIZE, row * CELL_SIZE}; }