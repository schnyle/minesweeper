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
      // copySprite(backBuffer, sprites.intToSpriteMap[col], x, y);
      copySprite(backBuffer, sprites.mine, x, y);
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
  makeEmptyCellSprite();
  makeHiddenCellSprite();
  makeFlaggedCellSprite();
  makeMineCellSprite();

  makeOneSprite();
  makeTwoSprite();
  makeThreeSprite();
  makeFourSprite();
  makeFiveSprite();
  makeSixSprite();
  makeSevenSprite();
  makeEightSprite();
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
      config::WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::WINDOW_PIXEL_HEIGHT - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
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
      config::WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
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
      config::WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
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
      config::WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
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
      config::WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
}

void RasterRenderer::makeEmptyCellSprite()
{
  auto &buff = sprites.empty;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
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

void RasterRenderer::makeMineCellSprite()
{
  auto &buff = sprites.mine;
  buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::RED);
  buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);

  // rest of this is AI - I'm too lazy to do more art for now

  // Draw the central black circle of the mine
  const int size = config::CELL_PIXEL_SIZE;
  const int center = size / 2;
  const int mineRadius = size / 4; // Mine circle radius
  const int spikeThickness = 2;
  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      int dx = x - center;
      int dy = y - center;
      if (dx * dx + dy * dy <= mineRadius * mineRadius)
      {
        buff[y * size + x] = 0xFF000000; // Black color
      }
    }
  }

  // Draw the central black circle of the mine
  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      int dx = x - center;
      int dy = y - center;
      if (dx * dx + dy * dy <= mineRadius * mineRadius)
      {
        buff[y * size + x] = 0xFF000000;
      }
    }
  }

  // Draw the 8 thicker spikes around the circle
  const int spikeLength = mineRadius + 4;
  const double directions[8][2] = {
      {1.0, 0.0},
      {0.707, 0.707},
      {0.0, 1.0},
      {-0.707, 0.707},
      {-1.0, 0.0},
      {-0.707, -0.707},
      {0.0, -1.0},
      {0.707, -0.707}};

  const int halfThick = spikeThickness / 2;

  for (const auto &dir : directions)
  {
    // Draw multiple parallel lines to create thickness
    for (int offset = -halfThick; offset <= halfThick; offset++)
    {
      double perpX = -dir[1]; // Perpendicular vector for thickness
      double perpY = dir[0];

      int startX = center + static_cast<int>(perpX * offset);
      int startY = center + static_cast<int>(perpY * offset);
      int endX = startX + static_cast<int>(dir[0] * spikeLength);
      int endY = startY + static_cast<int>(dir[1] * spikeLength);

      // Draw line from start to end point
      int dx = endX - startX;
      int dy = endY - startY;
      int steps = std::max(std::abs(dx), std::abs(dy));
      if (steps == 0)
        continue;

      for (int i = 0; i <= steps; i++)
      {
        int x = startX + dx * i / steps;
        int y = startY + dy * i / steps;

        // Draw additional pixels for thickness
        for (int px = -halfThick; px <= halfThick; px++)
        {
          for (int py = -halfThick; py <= halfThick; py++)
          {
            int finalX = x + px;
            int finalY = y + py;
            if (finalX >= 0 && finalX < size && finalY >= 0 && finalY < size)
            {
              // Only draw if we're outside the center circle
              int distX = finalX - center;
              int distY = finalY - center;
              if (distX * distX + distY * distY >= mineRadius * mineRadius)
              {
                buff[finalY * size + finalX] = 0xFF000000;
              }
            }
          }
        }
      }
    }
  }
}

void RasterRenderer::copyNumericSprite(uint32_t *dest, uint32_t *source)
{
  for (int i = 0; i < NUMERIC_SPRITE_SIZE; ++i)
  {
    const auto spriteStart = source + i * NUMERIC_SPRITE_SIZE;
    const auto spriteEnd = source + i * NUMERIC_SPRITE_SIZE + NUMERIC_SPRITE_SIZE;
    const int buffIdx = ((i + NUMERIC_SPRITE_PAD) * config::CELL_PIXEL_SIZE) + NUMERIC_SPRITE_PAD;
    std::copy(spriteStart, spriteEnd, dest + buffIdx);
  }
}

void RasterRenderer::makeOneSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  buffInsertRectangle(
      sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, NUMERIC_SPRITE_SIZE - baseHeight, baseWidth, baseHeight, config::BLUE);

  // stem
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, stemLeftPad, 0, stemWidth, NUMERIC_SPRITE_SIZE, config::BLUE);

  // topper
  const int topperWidth = 0.2 * NUMERIC_SPRITE_SIZE;
  const int topperHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int topperX = stemLeftPad - topperWidth;
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, topperX, 0, topperWidth, topperHeight, config::BLUE);

  auto &buff = sprites.one;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeTwoSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // bases
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  // bottom
  buffInsertRectangle(
      sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, NUMERIC_SPRITE_SIZE - baseHeight, baseWidth, baseHeight, config::GREEN);
  // top
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, baseWidth, baseHeight, config::GREEN);
  // middle
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      baseWidth,
      baseHeight,
      config::GREEN);

  // stems
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  // top
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      0,
      stemWidth,
      NUMERIC_SPRITE_SIZE / 2,
      config::GREEN);
  // bottom
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      stemWidth,
      NUMERIC_SPRITE_SIZE / 2,
      config::GREEN);

  auto &buff = sprites.two;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeThreeSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // bases
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  // bottom
  buffInsertRectangle(
      sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, NUMERIC_SPRITE_SIZE - baseHeight, baseWidth, baseHeight, config::RED);
  // top
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, baseWidth, baseHeight, config::RED);
  // middle
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      baseWidth,
      baseHeight,
      config::RED);

  // stem
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      0,
      stemWidth,
      NUMERIC_SPRITE_SIZE,
      config::RED);

  auto &buff = sprites.three;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeFourSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  buffInsertRectangle(
      sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, (NUMERIC_SPRITE_SIZE) / 3, baseWidth, baseHeight, config::DARK_BLUE);

  // stems
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  // right
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      0,
      stemWidth,
      NUMERIC_SPRITE_SIZE,
      config::DARK_BLUE);
  // left
  buffInsertRectangle(
      sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, stemWidth, NUMERIC_SPRITE_SIZE / 3, config::DARK_BLUE);

  auto &buff = sprites.four;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeFiveSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // bases
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  // bottom
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      NUMERIC_SPRITE_SIZE - baseHeight,
      baseWidth,
      baseHeight,
      config::DARK_RED);
  // top
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, baseWidth, baseHeight, config::DARK_RED);
  // middle
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      baseWidth,
      baseHeight,
      config::DARK_RED);

  // stems
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  // top
  buffInsertRectangle(
      sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, stemWidth, NUMERIC_SPRITE_SIZE / 2, config::DARK_RED);
  // bottom
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      stemWidth,
      NUMERIC_SPRITE_SIZE / 2,
      config::DARK_RED);

  auto &buff = sprites.five;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeSixSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // bases
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  // bottom
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      NUMERIC_SPRITE_SIZE - baseHeight,
      baseWidth,
      baseHeight,
      config::TURQUOISE);
  // top
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, baseWidth, baseHeight, config::TURQUOISE);
  // middle
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      baseWidth,
      baseHeight,
      config::TURQUOISE);

  // stems
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  // left
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, stemWidth, NUMERIC_SPRITE_SIZE, config::TURQUOISE);
  // right
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      stemWidth,
      NUMERIC_SPRITE_SIZE / 2,
      config::TURQUOISE);

  auto &buff = sprites.six;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeSevenSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // base
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, baseWidth, baseHeight, config::PURPLE);

  // stem
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      0,
      stemWidth,
      NUMERIC_SPRITE_SIZE,
      config::PURPLE);

  auto &buff = sprites.seven;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::makeEightSprite()
{
  uint32_t sprite[NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE];
  std::fill_n(sprite, NUMERIC_SPRITE_SIZE * NUMERIC_SPRITE_SIZE, config::GREY);

  // bases
  const int baseHeight = 0.15 * NUMERIC_SPRITE_SIZE;
  const int baseWidth = 0.6 * NUMERIC_SPRITE_SIZE;
  const int baseLeftPad = (NUMERIC_SPRITE_SIZE - baseWidth) / 2;
  // bottom
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      NUMERIC_SPRITE_SIZE - baseHeight,
      baseWidth,
      baseHeight,
      config::DARK_GREY);
  // top
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, baseWidth, baseHeight, config::DARK_GREY);
  // middle
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      baseLeftPad,
      (NUMERIC_SPRITE_SIZE - baseHeight) / 2,
      baseWidth,
      baseHeight,
      config::DARK_GREY);

  // stems
  const int stemWidth = 0.15 * NUMERIC_SPRITE_SIZE;
  const int stemLeftPad = (NUMERIC_SPRITE_SIZE - stemWidth) / 2;
  // left
  buffInsertRectangle(sprite, NUMERIC_SPRITE_SIZE, baseLeftPad, 0, stemWidth, NUMERIC_SPRITE_SIZE, config::DARK_GREY);
  // right
  buffInsertRectangle(
      sprite,
      NUMERIC_SPRITE_SIZE,
      NUMERIC_SPRITE_SIZE - NUMERIC_SPRITE_PAD,
      0,
      stemWidth,
      NUMERIC_SPRITE_SIZE,
      config::DARK_GREY);

  auto &buff = sprites.eight;
  std::copy(sprites.empty, sprites.empty + sprites.spriteSize, buff);
  copyNumericSprite(buff, sprite);
}

void RasterRenderer::copySprite(std::unique_ptr<uint32_t[]> &buff, const uint32_t *sprite, const int x, const int y)
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

void RasterRenderer::buffInsert2DBorder(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const uint32_t c)
{
  // left edge
  buffInsertRectangle(buff, buffWidth, x, y, config::CELL_BORDER_WIDTH_2D, h, c);
  // top edge
  buffInsertRectangle(buff, buffWidth, x, y, w, config::CELL_BORDER_WIDTH_2D, c);
  // right edge
  buffInsertRectangle(buff, buffWidth, w - config::CELL_BORDER_WIDTH_2D, y, config::CELL_BORDER_WIDTH_2D, h, c);
  // left edge
  buffInsertRectangle(buff, buffWidth, x, h - config::CELL_BORDER_WIDTH_2D, w, config::CELL_BORDER_WIDTH_2D, c);
}

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