#include <X11/Xlib.h>
#include <stdexcept>

#include "Renderer.hpp"
#include "data.hpp"

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

void Renderer::drawUnrevealedCell(int x, int y)
{
  short pixelXMin = x * CELL_SIZE;
  short pixelXMax = pixelXMin + CELL_SIZE - 1;
  short pixelYMin = y * CELL_SIZE;
  short pixelYMax = pixelYMin + CELL_SIZE - 1;

  // square
  XSetForeground(display, gc, GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_SIZE, CELL_SIZE);

  // top/left edges
  XSetForeground(display, gc, LIGHT_GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  // bottom/right edges
  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMax - CELL_BORDER_WIDTH_3D + 1, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pixelXMax - CELL_BORDER_WIDTH_3D + 1, pixelYMin, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  int SHAPE = Complex;
  int MODE = CoordModeOrigin;

  // bottom left corner
  // TODO do these explicit int -> short conversions better
  XPoint blPoints[3] = {
      {pixelXMin, short(pixelYMax - CELL_BORDER_WIDTH_3D)},
      {pixelXMin, pixelYMax},
      {short(pixelXMin + CELL_BORDER_WIDTH_3D), short(pixelYMax - CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, blPoints, 3, SHAPE, MODE);

  // top right corner
  XPoint trPoints[3] = {
      {short(pixelXMax - CELL_BORDER_WIDTH_3D), pixelYMin},
      {pixelXMax, pixelYMin},
      {short(pixelXMax - CELL_BORDER_WIDTH_3D), short(pixelYMin + CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, trPoints, 3, SHAPE, MODE);
}

void Renderer::drawRevealedCell(int x, int y, int n)
{
  int pixelXMin = x * CELL_SIZE;
  int pixelXMax = pixelXMin + CELL_SIZE - 1;
  int pixelYMin = y * CELL_SIZE;
  int pixelYMax = pixelYMin + CELL_SIZE - 1;

  // square
  XSetForeground(display, gc, GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_SIZE, CELL_SIZE);

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

  // edges
  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, pixelXMin - 1, pixelYMin - 1, CELL_SIZE + 1, CELL_BORDER_WIDTH_2D);
  XFillRectangle(display, window, gc, pixelXMin - 1, pixelYMin - 1, CELL_BORDER_WIDTH_2D, CELL_SIZE + 1);
}

void Renderer::drawFlaggedCell(int x, int y)
{
  short pixelXMin = x * CELL_SIZE;
  short pixelXMax = pixelXMin + CELL_SIZE - 1;
  short pixelYMin = y * CELL_SIZE;
  short pixelYMax = pixelYMin + CELL_SIZE - 1;

  // square
  XSetForeground(display, gc, GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_SIZE, CELL_SIZE);

  XImage *flag = XCreateImage(
      display,
      visual,
      DefaultDepth(display, screen),
      ZPixmap,
      0,
      (char *)flagPixel,
      CELL_SIZE,
      CELL_SIZE,
      32,
      CELL_SIZE * sizeof(uint32_t));

  // For each pixel in your flag image
  for (int i = 0; i < CELL_SIZE; i++)
  {
    for (int j = 0; j < CELL_SIZE; j++)
    {
      uint32_t pixel = flagPixel[i * CELL_SIZE + j];
      if (pixel != 0xFFFFFFFF)
      { // If pixel isn't white
        XSetForeground(display, gc, pixel);
        XDrawPoint(display, window, gc, pixelXMin + j, pixelYMin + i);
      }
    }
  }

  // XGCValues values;
  // GC flagGC = XCreateGC(display, window, 0, &values);
  // XSetFunction(display, flagGC, GXand);
  // XPutImage(display, window, flagGC, flag, 0, 0, pixelXMin, pixelYMin, CELL_SIZE, CELL_SIZE);

  // top/left edges
  XSetForeground(display, gc, LIGHT_GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  // bottom/right edges
  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMax - CELL_BORDER_WIDTH_3D + 1, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pixelXMax - CELL_BORDER_WIDTH_3D + 1, pixelYMin, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  int SHAPE = Complex;
  int MODE = CoordModeOrigin;

  // bottom left corner
  // TODO do these explicit int -> short conversions better
  XPoint blPoints[3] = {
      {pixelXMin, short(pixelYMax - CELL_BORDER_WIDTH_3D)},
      {pixelXMin, pixelYMax},
      {short(pixelXMin + CELL_BORDER_WIDTH_3D), short(pixelYMax - CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, blPoints, 3, SHAPE, MODE);

  // top right corner
  XPoint trPoints[3] = {
      {short(pixelXMax - CELL_BORDER_WIDTH_3D), pixelYMin},
      {pixelXMax, pixelYMin},
      {short(pixelXMax - CELL_BORDER_WIDTH_3D), short(pixelYMin + CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, trPoints, 3, SHAPE, MODE);
}

void Renderer::drawBoard(Cell (&data)[GRID_HEIGHT][GRID_WIDTH])
{
  for (size_t y = 0; y < GRID_HEIGHT; ++y)
  {
    for (size_t x = 0; x < GRID_WIDTH; ++x)
    {
      if (!data[y][x].isRevealed)
      {
        if (data[y][x].isFlagged)
        {
          drawFlaggedCell(x, y);
        }
        else
        {
          drawUnrevealedCell(x, y);
        }
      }
      else
      {
        drawRevealedCell(x, y, data[y][x].nAdjacentMines);
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
