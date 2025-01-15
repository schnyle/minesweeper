#include <RasterRenderer.hpp>
#include <X11/Xlib.h>
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

  while (true)
  {
    XNextEvent(display, &event);

    updateBackBuffer();

    renderFrame();
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
  const int bufferSize = config::WINDOW_PIXEL_HEIGHT * config::WINDOW_PIXEL_WIDTH * 4;
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
      0);
}

void RasterRenderer::renderFrame()
{
  image->data = reinterpret_cast<char *>(backBuffer.get());
  XPutImage(display, window, gc, image, 0, 0, 0, 0, config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_HEIGHT);
  frontBuffer.swap(backBuffer);
}

void RasterRenderer::updateBackBuffer()
{
  std::random_device rd;
  std::mt19937 rg(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (int row = 0; row < config::WINDOW_PIXEL_HEIGHT; ++row)
  {
    for (int col = 0; col < config::WINDOW_PIXEL_WIDTH; ++col)
    {
      backBuffer[rowColToIndex(row, col)] = dist(rg) * 0xffffff;
    }
  }
}

int RasterRenderer::rowColToIndex(const int row, const int col) const { return row * config::WINDOW_PIXEL_WIDTH + col; }