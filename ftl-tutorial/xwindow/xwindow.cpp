#include <X11/Xlib.h>
#include <stdexcept>

#define POS_Y 500
#define POS_X 500
#define WIDTH 500
#define HEIGHT 500
#define BORDER 15

static Display *display;
static int screen;
static Window root;

static Window createWindow(int x, int y, int w, int h, int b)
{
  Window window;
  XSetWindowAttributes xwa;

  xwa.background_pixel = WhitePixel(display, screen);
  xwa.border_pixel = BlackPixel(display, screen);
  xwa.event_mask = ButtonPress;

  window = XCreateWindow(
      display,
      root,
      x,
      y,
      w,
      h,
      b,
      DefaultDepth(display, screen),
      InputOutput,
      DefaultVisual(display, screen),
      CWBackPixel | CWBorderPixel | CWEventMask,
      &xwa);
  return window;
}

static void run()
{
  XEvent event;

  while (XNextEvent(display, &event) == 0)
  {
    switch (event.type)
    {
    case ButtonPress:
      return;
    }
  }
}

int main()
{
  Window window;

  display = XOpenDisplay(NULL);
  if (!display)
  {
    throw std::runtime_error("Can't open display");
    return 1;
  }

  /* get default screen and root window */
  screen = DefaultScreen(display);
  root = RootWindow(display, screen);

  window = createWindow(POS_X, POS_Y, WIDTH, HEIGHT, BORDER);

  XMapWindow(display, window);

  run();

  /* cleanup */
  XUnmapWindow(display, window);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}