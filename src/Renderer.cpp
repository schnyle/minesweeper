#include <Game.hpp>
#include <Renderer.hpp>
#include <X11/Xlib.h>
#include <config.hpp>
#include <data.hpp>
#include <fstream>
#include <iostream>
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
  XMapWindow(display, window);

  gc = createGC();

  loadImageData();

  XStoreName(display, window, config::APP_NAME);
}

Renderer::~Renderer() { XCloseDisplay(display); }

void Renderer::run(Game &game)
{
  XEvent event;

  while (true)
  {
    XNextEvent(display, &event);

    switch (event.type)
    {
    case Expose:
      drawBoard(game.getMinefield());
      break;
    case ButtonPress:
      const int row = event.xbutton.y / config::CELL_PIXEL_SIZE;
      const int col = event.xbutton.x / config::CELL_PIXEL_SIZE;

      if (event.xbutton.button == Button1)
      {
        game.handleLeftClick(row, col);
      }
      else if (event.xbutton.button == Button2)
      {
        game.handleMiddleClick(row, col);
      }
      else if (event.xbutton.button == Button3)
      {
        game.handleRightClick(row, col);
      }
      break;
    }

    drawBoard(game.getMinefield());
  }
}

void Renderer::drawCellBase(const int row, const int col) const
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, config::GREY);
  XFillRectangle(display, window, gc, p.x, p.y, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE);
}

void Renderer::drawMineCellBase(const int row, const int col) const
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, config::RED);
  XFillRectangle(display, window, gc, p.x, p.y, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE);
}

void Renderer::draw2DEdges(const int row, const int col) const
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, config::DARK_GREY);
  XFillRectangle(display, window, gc, p.x - 1, p.y - 1, config::CELL_PIXEL_SIZE + 1, config::CELL_BORDER_WIDTH_2D);
  XFillRectangle(display, window, gc, p.x - 1, p.y - 1, config::CELL_BORDER_WIDTH_2D, config::CELL_PIXEL_SIZE + 1);
}

void Renderer::draw3DEdges(const int row, const int col) const
{
  Point pMin = rowColToPixelPoint(row, col);
  Point pMax{pMin.x + config::CELL_PIXEL_SIZE - 1, pMin.y + config::CELL_PIXEL_SIZE - 1};

  // top/left edges
  XSetForeground(display, gc, config::LIGHT_GREY);
  XFillRectangle(display, window, gc, pMin.x, pMin.y, config::CELL_PIXEL_SIZE, config::CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pMin.x, pMin.y, config::CELL_BORDER_WIDTH_3D, config::CELL_PIXEL_SIZE);

  // bottom/right edges
  XSetForeground(display, gc, config::DARK_GREY);
  XFillRectangle(
      display,
      window,
      gc,
      pMin.x,
      pMax.y - config::CELL_BORDER_WIDTH_3D + 1,
      config::CELL_PIXEL_SIZE,
      config::CELL_BORDER_WIDTH_3D);
  XFillRectangle(
      display,
      window,
      gc,
      pMax.x - config::CELL_BORDER_WIDTH_3D + 1,
      pMin.y,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_PIXEL_SIZE);

  static int SHAPE = Complex;
  static int MODE = CoordModeOrigin;

  // bottom left corner
  XPoint blPoints[3] = {
      {static_cast<short>(pMin.x), static_cast<short>(pMax.y - config::CELL_BORDER_WIDTH_3D)},
      {static_cast<short>(pMin.x), static_cast<short>(pMax.y)},
      {static_cast<short>(pMin.x + config::CELL_BORDER_WIDTH_3D),
       static_cast<short>(pMax.y - config::CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, config::LIGHT_GREY);
  XFillPolygon(display, window, gc, blPoints, 3, SHAPE, MODE);

  // top right corner
  XPoint trPoints[3] = {
      {static_cast<short>(pMax.x - config::CELL_BORDER_WIDTH_3D), static_cast<short>(pMin.y)},
      {static_cast<short>(pMax.x), static_cast<short>(pMin.y)},
      {static_cast<short>(pMax.x - config::CELL_BORDER_WIDTH_3D),
       static_cast<short>(pMin.y + config::CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, config::LIGHT_GREY);
  XFillPolygon(display, window, gc, trPoints, 3, SHAPE, MODE);
}

void Renderer::drawHiddenCell(const int row, const int col) const
{
  drawCellBase(row, col);
  draw3DEdges(row, col);
}

void Renderer::drawRevealedCell(const int row, const int col) const
{
  drawCellBase(row, col);
  draw2DEdges(row, col);
}

void Renderer::drawAdjacentMinesNum(const int row, const int col, const int n) const
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

void Renderer::drawRevealedMineCell(const int row, const int col) const
{
  drawMineCellBase(row, col);
  overlayImage(row, col, images.mine);
}

void Renderer::drawBoard(const Game::Minefield &minefield) const
{
  for (size_t row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (size_t col = 0; col < config::GRID_WIDTH; ++col)
    {
      const size_t index = row * config::GRID_WIDTH + col;
      const auto &[isMine, isHidden, isFlagged, nAdjacentMines] = minefield[index];

      if (isHidden)
      {
        drawHiddenCell(row, col);

        if (isFlagged)
        {
          overlayImage(row, col, images.flag);
        }
      }
      else
      {
        if (isMine)
        {
          drawRevealedMineCell(row, col);
        }
        else
        {
          drawRevealedCell(row, col);
          drawAdjacentMinesNum(row, col, minefield[index].nAdjacentMines);
        }
      }
    }
  }
}

void Renderer::overlayImage(const int row, const int col, const char *image, uint32_t transparentHex) const
{
  Point p = rowColToPixelPoint(row, col);

  for (int i = 0; i < config::CELL_PIXEL_SIZE; ++i)
  {
    for (int j = 0; j < config::CELL_PIXEL_SIZE; ++j)
    {
      const char *pixelPtr = image + (i * config::CELL_PIXEL_SIZE + j) * 4;
      uint32_t pixel = *reinterpret_cast<const uint32_t *>(pixelPtr);
      if (pixel != transparentHex)
      {
        XSetForeground(display, gc, pixel);
        XDrawPoint(display, window, gc, p.x + j, p.y + i);
      }
    }
  }
}

void Renderer::loadBinaryFile(const std::string &filepath, char (&dest)[config::IMAGE_SIZE])
{
  std::ifstream file(filepath, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  file.read(dest, config::IMAGE_SIZE);
  if (file.gcount() != config::IMAGE_SIZE)
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

Renderer::Point Renderer::rowColToPixelPoint(const int row, const int col) const
{
  return {col * config::CELL_PIXEL_SIZE, row * config::CELL_PIXEL_SIZE};
}
