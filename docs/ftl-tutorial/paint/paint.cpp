// #include <X11/X.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdexcept>

#define POS_Y 500
#define POS_X 500
#define WIDTH 500
#define HEIGHT 500
#define BORDER 15
#define LINE 4

static Display *display;
static int screen;
static Window root;

static Window createWindow(int x, int y, int w, int h, int b)
{
  Window window;
  XSetWindowAttributes xwa = {};
  xwa.background_pixel = WhitePixel(display, screen);
  xwa.border_pixel = BlackPixel(display, screen);
  xwa.event_mask = Button1MotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask;

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
      CWBackPixel | CWEventMask | CWBorderPixel,
      &xwa);
  return window;
}

static GC createGraphicsContext(int lineWidth)
{
  GC gc;
  XGCValues xgcv;
  unsigned long valueMask;

  xgcv.line_style = LineSolid;
  xgcv.line_width = lineWidth;
  xgcv.cap_style = CapButt;
  xgcv.join_style = JoinMiter;
  xgcv.fill_style = FillSolid;
  xgcv.foreground = BlackPixel(display, screen);
  xgcv.background = WhitePixel(display, screen);

  valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCLineWidth | GCCapStyle | GCJoinStyle;
  gc = XCreateGC(display, root, valueMask, &xgcv);
  return gc;
}

static void run(GC gc)
{
  XEvent event;
  int init = 0;
  int prevX = 0, prevY = 0;

  while (!(XNextEvent(display, &event)))
  {
    switch (event.type)
    {
    case ButtonPress:
      if (event.xbutton.button == Button1)
      {
        XDrawPoint(display, event.xbutton.window, gc, event.xbutton.x, event.xbutton.y);
      }
      break;
    case MotionNotify:
      if (init)
      {
        XDrawLine(display, event.xbutton.window, gc, prevX, prevY, event.xbutton.x, event.xbutton.y);
      }
      else
      {
        XDrawPoint(display, event.xbutton.window, gc, event.xbutton.x, event.xbutton.y);
        init = 1;
      }
      prevX = event.xbutton.x, prevY = event.xbutton.y;
      break;
    case ButtonRelease:
      init = 0;
      break;
    case KeyPress:
      if (XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0) == XK_q)
      {
        return;
      }
    }
  }
}

int main()
{
  Window mainWindow;
  GC gc;
  if (!(display = XOpenDisplay(NULL)))
  {
    throw std::runtime_error("Can't open display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);

  mainWindow = createWindow(POS_X, POS_Y, WIDTH, HEIGHT, BORDER);
  gc = createGraphicsContext(LINE);

  XSizeHints xsh = {};
  xsh.min_width = WIDTH;
  xsh.max_width = WIDTH;
  xsh.min_height = HEIGHT;
  xsh.max_height = HEIGHT;
  xsh.flags = PMinSize | PMaxSize;
  XSetSizeHints(display, mainWindow, &xsh, XA_WM_NORMAL_HINTS);

  XStoreName(display, mainWindow, "Paint Program");

  XMapWindow(display, mainWindow);

  run(gc);

  /* cleanup */
  XUnmapWindow(display, mainWindow);
  XDestroyWindow(display, mainWindow);
  XFreeGC(display, gc);
  XCloseDisplay(display);

  return 0;
}