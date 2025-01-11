#include <X11/Xlib.h>
#include <array>

#include "data.hpp"

#define APP_NAME "Minesweeper"
#define CELL_SIZE 50
#define CELL_BORDER_WIDTH_3D 5
#define CELL_BORDER_WIDTH_2D 2 // even
#define DARK_GREY 0x7a7a7b
#define GREY 0xbdbdbd
#define LIGHT_GREY 0xfefffe

static Display *display;
static int screen;
static Window root;

GC createGC()
{
  GC gc;
  XGCValues xgcv;

  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  xgcv.join_style = JoinMiter;
  xgcv.fill_style = FillSolid;
  xgcv.foreground = BlackPixel(display, screen);
  xgcv.background = WhitePixel(display, screen);

  unsigned long valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCLineWidth | GCCapStyle |
                            GCJoinStyle;
  gc = XCreateGC(display, root, valueMask, &xgcv);
  return gc;
}

void drawUnrevealedCell(Display *display, Window window, GC gc, int x, int y)
{
  int pixelXMin = x * CELL_SIZE;
  int pixelXMax = pixelXMin + CELL_SIZE - 1;
  int pixelYMin = y * CELL_SIZE;
  int pixelYMax = pixelYMin + CELL_SIZE - 1;

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
  XPoint blPoints[3] = {
      {pixelXMin, pixelYMax - CELL_BORDER_WIDTH_3D},
      {pixelXMin, pixelYMax},
      {pixelXMin + CELL_BORDER_WIDTH_3D, pixelYMax - CELL_BORDER_WIDTH_3D}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, blPoints, 3, SHAPE, MODE);

  // top right corner
  XPoint trPoints[3] = {
      {pixelXMax - CELL_BORDER_WIDTH_3D, pixelYMin},
      {pixelXMax, pixelYMin},
      {pixelXMax - CELL_BORDER_WIDTH_3D, pixelYMin + CELL_BORDER_WIDTH_3D}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, trPoints, 3, SHAPE, MODE);
}

void drawEmptyCell(Display *display, Window window, GC gc, int x, int y)
{
  int pixelXMin = x * CELL_SIZE;
  int pixelXMax = pixelXMin + CELL_SIZE - 1;
  int pixelYMin = y * CELL_SIZE;
  int pixelYMax = pixelYMin + CELL_SIZE - 1;

  // square
  XSetForeground(display, gc, GREY);
  XFillRectangle(display, window, gc, pixelXMin, pixelYMin, CELL_SIZE, CELL_SIZE);

  // edges
  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, pixelXMin - 1, pixelYMin - 1, CELL_SIZE + 1, CELL_BORDER_WIDTH_2D);
  XFillRectangle(display, window, gc, pixelXMin - 1, pixelYMin - 1, CELL_BORDER_WIDTH_2D, CELL_SIZE + 1);
}

void drawBoard(Display *display, Window window, GC gc, Cell (&data)[GRID_HEIGHT][GRID_WIDTH])
{
  for (size_t y = 0; y < GRID_HEIGHT; ++y)
  {
    for (size_t x = 0; x < GRID_WIDTH; ++x)
    {
      if (!data[y][x].isRevealed)
      {
        drawUnrevealedCell(display, window, gc, x, y);
      }
      if (!data[y][x].nAdjacentMines)
      {
        drawEmptyCell(display, window, gc, x, y);
      }
    }
  }
}

int main()
{
  display = XOpenDisplay(nullptr);
  if (!display)
  {
    return 1;
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  Window window = XCreateSimpleWindow(
      display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));

  GC gc = createGC();

  XStoreName(display, window, APP_NAME);
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  XEvent event;
  while (true)
  {
    drawBoard(display, window, gc, data);
    XNextEvent(display, &event);
  }

  XCloseDisplay(display);

  return 0;
}