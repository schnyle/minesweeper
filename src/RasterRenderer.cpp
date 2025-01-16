#include <RasterRenderer.hpp>
#include <X11/Xlib.h>
#include <algorithm>
#include <config.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

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
      const int gameAreaWidth = config::WINDOW_PIXEL_WIDTH - 2 * config::FRAME_WIDTH;
      const int gameAreaHeight = config::WINDOW_PIXEL_HEIGHT - 2 * config::FRAME_WIDTH - config::INFO_PANEL_HEIGHT;
      const int gameAreaX = config::FRAME_WIDTH;
      const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

      const int x = gameAreaX + col * config::CELL_PIXEL_SIZE;
      const int y = gameAreaY + row * config::CELL_PIXEL_SIZE;
      copySprite(backBuffer, sprites.hidden, x, y);
      // return;
      // copySprite(backBuffer, sprites.flag, x, y);
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

  makeInterface();

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

void RasterRenderer::initializeSprites()
{
  makeHiddenCellSprite();
  makeFlaggedCellSprite();
};

void RasterRenderer::makeInterface()
{
  // base
  std::fill_n(backBuffer.get(), config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT, config::GREY);

  // border
  buffInsert3DBorder(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      0,
      0,
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);

  buffInsert3DBorder(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::WINDOW_PIXEL_WIDTH - 2 * config::FRAME_WIDTH,
      config::WINDOW_PIXEL_HEIGHT - 2 * config::FRAME_WIDTH,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);

  makeInfoPanel();
}

void RasterRenderer::makeInfoPanel()
{
  // interface / game separator
  buffInsert3DBorder(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::WINDOW_PIXEL_WIDTH - 2 * config::FRAME_WIDTH,
      config::FRAME_WIDTH,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  // fill overlap
  buffInsertRectangle(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + config::CELL_BORDER_WIDTH_3D,
      config::WINDOW_PIXEL_WIDTH - 2 * config::FRAME_WIDTH,
      config::FRAME_WIDTH - 2 * config::CELL_BORDER_WIDTH_3D,
      config::GREY);
  // top-left corner
  buffInsert3DCorner(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
  // top-right corner
  buffInsertRectangle(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH - 2 * config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::LIGHT_GREY);
  // bottom-left corner
  buffInsertRectangle(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY);
  // bottom-right corner
  buffInsert3DCorner(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH - 2 * config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
}

void RasterRenderer::makeHiddenCellSprite()
{
  auto &buff = sprites.hidden;

  // base
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);

  const auto interiorLimit = config::CELL_PIXEL_SIZE - config::CELL_BORDER_WIDTH_3D;
  buffInsert3DBorder(
      buff,
      config::CELL_PIXEL_SIZE,
      0,
      0,
      config::CELL_PIXEL_SIZE,
      config::CELL_PIXEL_SIZE,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
}

void RasterRenderer::makeFlaggedCellSprite()
{

  const int totalFlagPoleHeight = config::FLAG_TOTAL_HEIGHT_RATIO * config::CELL_PIXEL_SIZE;

  const int flagPoleBottomY = config::CELL_PIXEL_SIZE - ((config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2);

  auto &buff = sprites.flag;
  std::copy(sprites.hidden, sprites.hidden + sprites.spriteSize, buff);

  // bottom base rectangle
  const int bottomBaseRectHeight = config::FLAG_BOTTOM_BASE_HEIGHT_RATIO * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectWidth = config::FLAG_BOTTOM_BASE_WIDTH_RATIO * config::CELL_PIXEL_SIZE;
  const int bottomBaseRectX = (config::CELL_PIXEL_SIZE - bottomBaseRectWidth) / 2;
  const int bottomBaseRectY = flagPoleBottomY - bottomBaseRectHeight;
  buffInsertRectangle(
      buff,
      config::CELL_PIXEL_SIZE,
      bottomBaseRectX,
      bottomBaseRectY,
      bottomBaseRectWidth,
      bottomBaseRectHeight,
      config::BLACK);

  // top base rectangle
  const int topBaseRectHeight = config::FLAG_TOP_BASE_RECT_HEIGHT_RATIO * config::CELL_PIXEL_SIZE;
  const int topBaseRectWidth = config::FLAG_TOP_BASE_RECT_WIDTH_RATIO * config::CELL_PIXEL_SIZE;
  const int topBaseRectX = (config::CELL_PIXEL_SIZE - topBaseRectWidth) / 2;
  const int topBaseRectY = flagPoleBottomY - bottomBaseRectHeight - topBaseRectHeight;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, topBaseRectX, topBaseRectY, topBaseRectWidth, topBaseRectHeight, config::BLACK);

  // pole
  const int poleWidth = config::FLAG_POLE_WIDTH_RATIO * config::CELL_PIXEL_SIZE;
  const int poleX = (config::CELL_PIXEL_SIZE - poleWidth) / 2;
  const int poleY = (config::CELL_PIXEL_SIZE - totalFlagPoleHeight) / 2;
  buffInsertRectangle(buff, config::CELL_PIXEL_SIZE, poleX, poleY, poleWidth, totalFlagPoleHeight, config::BLACK);

  // flag
  const int flagSize = config::FLAG_SIZE_RATIO * config::CELL_PIXEL_SIZE;
  const int flagX = poleX + poleWidth - flagSize;
  const int flagY = poleY;
  const double flagSlope = config::FLAG_SLOPE;
  for (int y = 0; y < flagSize; ++y)
  {
    const int buffY = y + flagY;
    for (int x = 0; x < flagSize; ++x)
    {
      if (y <= (flagSlope * x) + (flagSize / 2) - 1 && y >= -(flagSlope * x) + (flagSize / 2) + 1)
      {
        const int buffX = x + flagX;
        buff[buffY * config::CELL_PIXEL_SIZE + buffX] = config::RED;
      }
    }
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
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t c)
{
  for (int row = y; row < y + h; ++row)
  {
    const int index = row * buffWidth + x;
    std::fill_n(buff + index, w, c);
  }
};

void RasterRenderer::buffInsert3DBorder(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t cTop,
    const uint32_t cMid,
    const uint32_t cBot)
{
  // left edge
  buffInsertRectangle(buff, buffWidth, x, y, config::CELL_BORDER_WIDTH_3D, h, cTop);
  // top edge
  buffInsertRectangle(buff, buffWidth, x, y, w, config::CELL_BORDER_WIDTH_3D, cTop);
  // right edge
  buffInsertRectangle(buff, buffWidth, x + w - config::CELL_BORDER_WIDTH_3D, y, config::CELL_BORDER_WIDTH_3D, h, cBot);
  // bottom edge
  buffInsertRectangle(buff, buffWidth, x, y + h - config::CELL_BORDER_WIDTH_3D, w, config::CELL_BORDER_WIDTH_3D, cBot);
  // top-right corner
  buffInsert3DCorner(
      buff,
      buffWidth,
      x + w - config::CELL_BORDER_WIDTH_3D,
      y,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      cTop,
      cMid,
      cBot);
  // bottom-left corner
  buffInsert3DCorner(
      buff,
      buffWidth,
      x,
      y + h - config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      cTop,
      cMid,
      cBot);
}

void RasterRenderer::buffInsert3DCorner(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t cTop,
    const uint32_t cMid,
    const uint32_t cBot)
{
  for (int row = y; row < y + h; ++row)
  {
    for (int col = x; col < x + w; ++col)
    {
      const auto idx = row * buffWidth + col;

      const int relY = row - y;
      const int relX = col - x;

      uint32_t color;
      const int diagonal = -relX + h - 1;
      if (relY == diagonal)
      {
        color = cMid;
      }
      else if (relY < diagonal)
      {
        color = cTop;
      }
      else
      {
        color = cBot;
      }

      buff[idx] = color;
    }
  }
}

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