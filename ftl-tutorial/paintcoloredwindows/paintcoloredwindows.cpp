// #include <X11/X.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdexcept>

#define POS_Y 500
#define POS_X 500
#define WIDTH 500
#define HEIGHT 500
#define BORDER 15
#define CHILD_WIDTH 50
#define CHILD_HEIGHT 50
#define CHILD_BORDER 5
#define LINE 4
#define BL "#0000ff"
#define GR "#00ff00"
#define RE "#ff0000"

enum
{
  BLUE,
  GREEN,
  RED
};

typedef struct
{
  Window window;
  XftColor *color;
} ColorButtons;

static Display *display;
static int screen;
static Window root;
static Visual *visual;
static ColorButtons colorButtons[3];
static const char *colors[3] = {BL, GR, RE};

// static void createColor(XftColor *colorBlue, const char *name)
// {
//   if (!XftColorAllocName(display, visual, DefaultColormap(display, screen), name, colorBlue))
//   {
//     throw std::runtime_error("can't allocate color");
//   };

//   colorBlue->pixel |= 0xff << 24;
// }

static Window createWindow(int x, int y, int w, int h, int b, Window *parent, int idx)
{
  Window window;
  XSetWindowAttributes xwa = {};
  xwa.background_pixel = WhitePixel(display, screen);
  xwa.border_pixel = BlackPixel(display, screen);

  if (!parent)
  {
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
        visual,
        CWBackPixel | CWEventMask | CWBorderPixel,
        &xwa);
  }
  else
  {
    xwa.event_mask = ButtonPress;
    xwa.background_pixel = colorButtons[idx].color->pixel;

    window = XCreateWindow(
        display,
        *parent,
        x,
        y,
        w,
        h,
        b,
        DefaultDepth(display, screen),
        InputOutput,
        visual,
        CWBackPixel | CWEventMask | CWBorderPixel,
        &xwa);
  }

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
  Window currentWindow;
  int init = 0;
  int prevX = 0, prevY = 0;

  while (!(XNextEvent(display, &event)))
  {
    switch (event.type)
    {
    case ButtonPress:
      if (event.xbutton.button == Button1)
      {
        currentWindow = event.xbutton.window;

        if (currentWindow == colorButtons[BLUE].window)
        {
          XSetForeground(display, gc, colorButtons[BLUE].color->pixel);
        }
        else if (currentWindow == colorButtons[RED].window)
        {
          XSetForeground(display, gc, colorButtons[RED].color->pixel);
        }
        else if (currentWindow == colorButtons[GREEN].window)
        {
          XSetForeground(display, gc, colorButtons[GREEN].color->pixel);
        }
        else
        {
          XDrawPoint(display, event.xbutton.window, gc, event.xbutton.x, event.xbutton.y);
        }
      }
      else if (event.xbutton.button == Button3)
      {
        XSetForeground(display, gc, BlackPixel(display, screen));
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
  int y = 0;

  if (!(display = XOpenDisplay(NULL)))
  {
    throw std::runtime_error("Can't open display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  visual = DefaultVisual(display, screen);
  mainWindow = createWindow(POS_X, POS_Y, WIDTH, HEIGHT, BORDER, nullptr, 0);

  for (int i = 0; i < 3; ++i)
  {
    colorButtons[i].color = new XftColor;

    if (!colorButtons[i].color)
    {
      throw std::runtime_error("can't allocate memory for color");
    }

    if (!XftColorAllocName(display, visual, DefaultColormap(display, screen), colors[i], colorButtons[i].color))
    {
      throw std::runtime_error("can't allocate xft color");
    }

    colorButtons[i].color->pixel |= 0xff << 24;
    colorButtons[i].window = createWindow(0, y, CHILD_WIDTH, CHILD_HEIGHT, CHILD_BORDER, &mainWindow, i);
    y += CHILD_HEIGHT + CHILD_BORDER;
  }

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
  XMapSubwindows(display, mainWindow);

  run(gc);

  /* cleanup */
  XUnmapWindow(display, mainWindow);
  XUnmapSubwindows(display, mainWindow);
  XDestroyWindow(display, mainWindow);
  XDestroySubwindows(display, mainWindow);
  XFreeGC(display, gc);

  for (int i = 0; i < 3; ++i)
  {
    XftColorFree(display, visual, DefaultColormap(display, screen), colorButtons[i].color);
  }

  XCloseDisplay(display);

  return 0;
}