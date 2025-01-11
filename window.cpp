// g++ -o window window.cpp -lX11

#include <X11/Xlib.h>

int main()
{
  Display *display = XOpenDisplay(nullptr);
  if (!display)
  {
    return 1;
  }

  int screen = DefaultScreen(display);
  Window window = XCreateSimpleWindow(
      display,
      RootWindow(display, screen),
      0, 0,
      800, 600,
      1,
      BlackPixel(display, screen),
      WhitePixel(display, screen));

  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  XEvent event;
  while (true)
  {
    XNextEvent(display, &event);
  }

  XCloseDisplay(display);

  return 0;
}