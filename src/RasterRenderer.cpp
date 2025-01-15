#include <RasterRenderer.hpp>
#include <X11/Xlib.h>
#include <algorithm>
#include <config.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <random>

RasterRenderer::RasterRenderer()
{
  display = XOpenDisplay(nullptr);
  if (!display)
  {
    throw std::runtime_error("unable to open X display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  visual = DefaultVisual(display, screen);

  XSetWindowAttributes xwa = {};
  xwa.background_pixel = WhitePixel(display, screen);
  xwa.border_pixel = BlackPixel(display, screen);
  xwa.event_mask = ExposureMask | Button1MotionMask | ButtonPressMask | ButtonReleaseMask; // check these
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
      &xwa);
  XMapWindow(display, window);

  XStoreName(display, window, config::APP_NAME);

  initializeGC();
  initializeBuffers();
  initializeSprites();
}

RasterRenderer::~RasterRenderer()
{
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

void RasterRenderer::run(Game &game)
{
  XEvent event;

  updateBackBuffer(game);

  while (true)
  {
    XNextEvent(display, &event);
    if (event.type == Expose)
    {
      renderFrame();
      continue;
    }

    updateGameState(game, event);
    updateBackBuffer(game);
    renderFrame();
  }
}

void RasterRenderer::renderFrame()
{
  image->data = reinterpret_cast<char *>(backBuffer.get());
  XPutImage(display, window, gc, image, 0, 0, 0, 0, config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_HEIGHT);
  frontBuffer.swap(backBuffer);
}

void RasterRenderer::updateGameState(Game &game, XEvent &event)
{
  switch (event.type)
  {
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
}

void RasterRenderer::updateBackBuffer(Game &game)
{
  for (int row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (int col = 0; col < config::GRID_WIDTH; ++col)
    {
      const int x = col * config::CELL_PIXEL_SIZE;
      const int y = row * config::CELL_PIXEL_SIZE;
      copySprite(backBuffer, sprites.hidden, x, y);
    }
  }
}

void RasterRenderer::initializeGC()
{
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
}

void RasterRenderer::initializeBuffers()
{
  const int bufferSize = config::WINDOW_PIXEL_HEIGHT * config::WINDOW_PIXEL_WIDTH;
  frontBuffer = std::make_unique<uint32_t[]>(bufferSize);
  backBuffer = std::make_unique<uint32_t[]>(bufferSize);

  image = XCreateImage(
      display,
      visual,
      DefaultDepth(display, screen),
      ZPixmap,
      0,
      reinterpret_cast<char *>(frontBuffer.get()),
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_HEIGHT,
      32,
      config::WINDOW_PIXEL_WIDTH * sizeof(int32_t));
}

void RasterRenderer::initializeSprites() { makeHiddenCellSprite(); };

void RasterRenderer::makeHiddenCellSprite()
{
  auto &buff = sprites.hidden;

  // top/left edges
  buffInsertRectangle(buff, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  buffInsertRectangle(buff, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_BORDER_WIDTH_3D, config::LIGHT_GREY);

  // bottom/right edges
  const auto interiorLimit = config::CELL_PIXEL_SIZE - config::CELL_BORDER_WIDTH_3D;
  buffInsertRectangle(buff, 0, 0, config::CELL_BORDER_WIDTH_3D, config::CELL_PIXEL_SIZE, config::LIGHT_GREY);
  buffInsertRectangle(buff, interiorLimit, 0, config::CELL_BORDER_WIDTH_3D, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  buffInsertRectangle(buff, 0, interiorLimit, config::CELL_PIXEL_SIZE, config::CELL_BORDER_WIDTH_3D, config::DARK_GREY);

  // top right corner
  for (int row = 0; row < config::CELL_BORDER_WIDTH_3D; ++row)
  {
    for (int col = 0; col < config::CELL_BORDER_WIDTH_3D - row - 1; ++col)
    {
      const auto idx = rowColToCellIndex(row, col + interiorLimit);
      buff[idx] = config::LIGHT_GREY;
    }

    const auto diagonalIdx = rowColToCellIndex(row, -row + config::CELL_PIXEL_SIZE - 1);
    buff[diagonalIdx] = config::GREY;
  }

  // bottom left corner
  for (int row = 0; row < config::CELL_BORDER_WIDTH_3D; ++row)
  {
    for (int col = 0; col < config::CELL_BORDER_WIDTH_3D - row - 1; ++col)
    {
      const auto idx = rowColToCellIndex(row + interiorLimit, col);
      buff[idx] = config::LIGHT_GREY;
    }

    const auto diagonalIdx = rowColToCellIndex(-row + config::CELL_PIXEL_SIZE - 1, row);
    buff[diagonalIdx] = config::GREY;
  }
}

void RasterRenderer::copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t (&sprite)[], const int x, const int y)
{
  for (int row = 0; row < config::CELL_PIXEL_SIZE; ++row)
  {
    const auto sourceRow = sprite + rowColToCellIndex(row, 0);
    const auto sourceRowEnd = sprite + rowColToCellIndex(row, config::CELL_PIXEL_SIZE);
    const auto destinationRow = buff.get() + rowColToWindowIndex(row + y, x);
    std::copy(sourceRow, sourceRowEnd, destinationRow);
  }
}

void RasterRenderer::buffInsertRectangle(
    uint32_t (&buff)[],
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t c)
{
  for (int row = y; row < y + h; ++row)
  {
    std::fill_n(buff + rowColToCellIndex(row, x), w, c);
  }
};

int RasterRenderer::rowColToWindowIndex(const int row, const int col) const
{
  return row * config::WINDOW_PIXEL_WIDTH + col;
}

int RasterRenderer::rowColToCellIndex(const int row, const int col) const
{
  return row * config::CELL_PIXEL_SIZE + col;
}

std::pair<int, int> RasterRenderer::rowColToPixelPoint(const int row, const int col) const
{
  return {col * config::CELL_PIXEL_SIZE, row * config::CELL_PIXEL_SIZE};
};