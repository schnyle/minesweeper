#include <X11/Xlib.h>
#include <fstream>
#include <minesweeper/Renderer.hpp>
#include <minesweeper/data.hpp>
#include <stdexcept>

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
  window = XCreateSimpleWindow(
      display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
  gc = createGC();

  loadImageData();
}

Renderer::~Renderer() { XCloseDisplay(display); }

void Renderer::render()
{
  XStoreName(display, window, APP_NAME);
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  XEvent event;
  while (true)
  {
    drawBoard(data);
    XNextEvent(display, &event);
  }
}

void Renderer::drawCellBase(int row, int col)
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, GREY);
  XFillRectangle(display, window, gc, p.x, p.y, CELL_SIZE, CELL_SIZE);
}

void Renderer::draw2DEdges(int row, int col)
{
  const Point p = rowColToPixelPoint(row, col);

  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, p.x - 1, p.y - 1, CELL_SIZE + 1, CELL_BORDER_WIDTH_2D);
  XFillRectangle(display, window, gc, p.x - 1, p.y - 1, CELL_BORDER_WIDTH_2D, CELL_SIZE + 1);
}

void Renderer::draw3DEdges(int row, int col)
{
  Point pMin = rowColToPixelPoint(row, col);
  Point pMax{pMin.x + CELL_SIZE - 1, pMin.y + CELL_SIZE - 1};

  // top/left edges
  XSetForeground(display, gc, LIGHT_GREY);
  XFillRectangle(display, window, gc, pMin.x, pMin.y, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pMin.x, pMin.y, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  // bottom/right edges
  XSetForeground(display, gc, DARK_GREY);
  XFillRectangle(display, window, gc, pMin.x, pMax.y - CELL_BORDER_WIDTH_3D + 1, CELL_SIZE, CELL_BORDER_WIDTH_3D);
  XFillRectangle(display, window, gc, pMax.x - CELL_BORDER_WIDTH_3D + 1, pMin.y, CELL_BORDER_WIDTH_3D, CELL_SIZE);

  static int SHAPE = Complex;
  static int MODE = CoordModeOrigin;

  // bottom left corner
  XPoint blPoints[3] = {
      {static_cast<short>(pMin.x), static_cast<short>(pMax.y - CELL_BORDER_WIDTH_3D)},
      {static_cast<short>(pMin.x), static_cast<short>(pMax.y)},
      {static_cast<short>(pMin.x + CELL_BORDER_WIDTH_3D), static_cast<short>(pMax.y - CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, blPoints, 3, SHAPE, MODE);

  // top right corner
  XPoint trPoints[3] = {
      {static_cast<short>(pMax.x - CELL_BORDER_WIDTH_3D), static_cast<short>(pMin.y)},
      {static_cast<short>(pMax.x), static_cast<short>(pMin.y)},
      {static_cast<short>(pMax.x - CELL_BORDER_WIDTH_3D), static_cast<short>(pMin.y + CELL_BORDER_WIDTH_3D)}};
  XSetForeground(display, gc, LIGHT_GREY);
  XFillPolygon(display, window, gc, trPoints, 3, SHAPE, MODE);
}

void Renderer::drawHiddenCell(int row, int col)
{
  drawCellBase(row, col);
  draw3DEdges(row, col);
}

void Renderer::drawRevealedCell(int row, int col, int n)
{
  drawCellBase(row, col);
  draw2DEdges(row, col);

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

void Renderer::overlayImage(int row, int col, const char *image, uint32_t transparentHex)
{
  Point p = rowColToPixelPoint(row, col);

  for (int i = 0; i < CELL_SIZE; ++i)
  {
    for (int j = 0; j < CELL_SIZE; ++j)
    {
      const char *pixelPtr = image + (i * CELL_SIZE + j) * 4;
      uint32_t pixel = *reinterpret_cast<const uint32_t *>(pixelPtr);
      if (pixel != transparentHex)
      {
        XSetForeground(display, gc, pixel);
        XDrawPoint(display, window, gc, p.x + j, p.y + i);
      }
    }
  }
}

void Renderer::drawBoard(Cell (&data)[GRID_HEIGHT][GRID_WIDTH])
{
  for (size_t row = 0; row < GRID_HEIGHT; ++row)
  {
    for (size_t col = 0; col < GRID_WIDTH; ++col)
    {
      if (data[row][col].isHidden)
      {
        drawHiddenCell(row, col);

        if (data[row][col].isFlagged)
        {
          overlayImage(row, col, images.flag);
        }
      }
      else
      {
        drawRevealedCell(row, col, data[row][col].nAdjacentMines);
      }
    }
  }
}

void Renderer::loadBinaryFile(const std::string &filepath, char (&dest)[IMAGE_SIZE])
{
  std::ifstream file(filepath, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  file.read(dest, IMAGE_SIZE);
  if (file.gcount() != IMAGE_SIZE)
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

Renderer::Point Renderer::rowColToPixelPoint(int row, int col) { return {col * CELL_SIZE, row * CELL_SIZE}; }