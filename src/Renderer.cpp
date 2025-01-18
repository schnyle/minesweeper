#include <Renderer.hpp>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <config.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

Renderer::Renderer()
{
  display = XOpenDisplay(nullptr);
  if (!display)
  {
    throw std::runtime_error("unable to open X display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  visual = DefaultVisual(display, screen);

  XSetWindowAttributes windowAttributes = {};
  windowAttributes.background_pixel = WhitePixel(display, screen);
  windowAttributes.border_pixel = BlackPixel(display, screen);
  windowAttributes.event_mask = ExposureMask | Button1MotionMask | ButtonPressMask | ButtonReleaseMask |
                                KeyPressMask; // check these
  windowAttributes.override_redirect = true;
  window = XCreateWindow(
      display,
      root,
      0,
      0,
      1920,
      1080,
      0,
      DefaultDepth(display, screen),
      InputOutput,
      visual,
      CWBackPixel | CWEventMask | CWBorderPixel,
      &windowAttributes);

  // remove header decoration
  Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", true);
  XSetWMProtocols(display, window, &wm_delete, 1);
  Atom wm_hints = XInternAtom(display, "_MOTIF_WM_HINTS", true);
  if (wm_hints != None)
  {
    struct
    {
      unsigned long flags;
      unsigned long functions;
      unsigned long decorations;
      long input_mode;
      unsigned long status;
    } hints = {2, 0, 0, 0, 0};
    XChangeProperty(display, window, wm_hints, wm_hints, 32, PropModeReplace, (unsigned char *)&hints, 5);
  }

  XMapWindow(display, window);

  XStoreName(display, window, config::APP_NAME);

  initializeGC();
  initializeBuffers();
  sprites = SpriteFactory::createSprites();
}

Renderer::~Renderer()
{
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

void Renderer::run(Game &game)
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

    if (!updateGameState(game, event))
    {
      break;
    };

    updateBackBuffer(game);
    renderFrame();
  }
}

void Renderer::renderFrame()
{
  image->data = reinterpret_cast<char *>(backBuffer.get());
  XPutImage(display, window, gc, image, 0, 0, 0, 0, config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_HEIGHT);
  frontBuffer.swap(backBuffer);
}

bool Renderer::updateGameState(Game &game, XEvent &event)
{
  switch (event.type)
  {
  case ButtonPress:
  {
    const int gameAreaX = config::FRAME_WIDTH;
    const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

    const int row = (event.xbutton.y - gameAreaY) / config::CELL_PIXEL_SIZE;
    const int col = (event.xbutton.x - gameAreaX) / config::CELL_PIXEL_SIZE;

    if (row < 0 || row > config::GRID_HEIGHT || col < 0 || col > config::GRID_WIDTH)
    {
      break;
    }

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

  case KeyPress:
    if (XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0) == XK_q)
    {
      return false;
    }
  }

  return true;
}

void Renderer::updateBackBuffer(Game &game)
{
  for (int row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (int col = 0; col < config::GRID_WIDTH; ++col)
    {
      const int gameAreaX = config::FRAME_WIDTH;
      const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

      const int x = gameAreaX + col * config::CELL_PIXEL_SIZE;
      const int y = gameAreaY + row * config::CELL_PIXEL_SIZE;

      const int index = row * config::GRID_WIDTH + col;
      const auto &[isMine, isHidden, isFlagged, nAdjacentMines] = game.getMinefield()[index];
      uint32_t *sprite;

      if (isHidden)
      {
        sprite = sprites->hidden;

        if (isFlagged)
        {
          sprite = sprites->flag;
        }
      }
      else
      {
        if (isMine)
        {
          sprite = sprites->mine;
        }
        else
        {
          sprite = sprites->intToSpriteMap[nAdjacentMines];
        }
      }

      SpriteFactory::copySprite(backBuffer, sprite, x, y);
    }
  }
}

void Renderer::initializeGC()
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

void Renderer::initializeBuffers()
{
  const int bufferSize = config::WINDOW_PIXEL_HEIGHT * config::WINDOW_PIXEL_WIDTH;
  frontBuffer = std::make_unique<uint32_t[]>(bufferSize);
  backBuffer = std::make_unique<uint32_t[]>(bufferSize);

  SpriteFactory::buffInsertInterface(
      backBuffer.get(), config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT);
  SpriteFactory::buffInsertInterface(
      frontBuffer.get(), config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT);

  image = XCreateImage(
      display,
      visual,
      DefaultDepth(display, screen),
      ZPixmap,
      0,
      reinterpret_cast<char *>(backBuffer.get()),
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_HEIGHT,
      32,
      config::WINDOW_PIXEL_WIDTH * sizeof(int32_t));
}