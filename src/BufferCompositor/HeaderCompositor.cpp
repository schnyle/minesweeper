#include <HeaderCompositor.hpp>
#include <algorithm>
#include <config.hpp>

void HeaderCompositor::buffInsertHeader(uint32_t *buff, const int buffWidth, const int buffSize)
{
  // base
  std::fill_n(buff, buffSize, config::GREY);

  // border
  BufferCompositor::buffInsert3DBorder(
      buff,
      buffWidth,
      0,
      0,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::GAME_WINDOW_PIXEL_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);

  BufferCompositor::buffInsert3DBorder(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::GAME_WINDOW_PIXEL_HEIGHT - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);

  // header / game separator
  BufferCompositor::buffInsert3DBorder(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::FRAME_WIDTH,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  // fill overlap
  BufferCompositor::buffInsertRectangle(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + config::CELL_BORDER_WIDTH_3D,
      config::GAME_WINDOW_PIXEL_WIDTH - 2 * (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::FRAME_WIDTH - 2 * config::CELL_BORDER_WIDTH_3D,
      config::GREY);
  // top-left corner
  BufferCompositor::buffInsert3DCorner(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
  // top-right corner
  BufferCompositor::buffInsertRectangle(
      buff,
      buffWidth,
      config::GAME_WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::LIGHT_GREY);
  // bottom-left corner
  BufferCompositor::buffInsertRectangle(
      buff,
      buffWidth,
      config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY);
  // bottom-right corner
  BufferCompositor::buffInsert3DCorner(
      buff,
      buffWidth,
      config::GAME_WINDOW_PIXEL_WIDTH - config::FRAME_WIDTH,
      config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + (config::FRAME_WIDTH - config::CELL_BORDER_WIDTH_3D),
      config::CELL_BORDER_WIDTH_3D,
      config::CELL_BORDER_WIDTH_3D,
      config::DARK_GREY,
      config::GREY,
      config::LIGHT_GREY);
};

void HeaderCompositor::buffInsertRemainingFlags(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const int n)
{
  BufferCompositor::buffInsertRectangle(buff, buffWidth, x, y, w, h, config::BLACK);

  const int leftDigit = n / 100;
  const int middleDigit = (n % 100) / 10;
  const int rightDigit = (n % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::REMAINING_FLAGS_PAD;
  while ((w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (w - 4 * pad) / 3;
  const int digitHeight = h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = x + pad + i * (digitWidth + pad);
    const int digitY = y + pad;

    buffInsertDigit(buff, buffWidth, digitX, digitY, digitWidth, digitHeight, digits[i], config::RED);
  }
};

void HeaderCompositor::buffInsertTimer(
    uint32_t *buff,
    const int buffWidth,
    const int x,
    const int y,
    const int w,
    const int h,
    const int t)
{
  BufferCompositor::buffInsertRectangle(buff, buffWidth, x, y, w, h, config::BLACK);

  const int leftDigit = t / 100;
  const int middleDigit = (t % 100) / 10;
  const int rightDigit = (t % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::REMAINING_FLAGS_PAD;
  while ((w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (w - 4 * pad) / 3;
  const int digitHeight = h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = x + pad + i * (digitWidth + pad);
    const int digitY = y + pad;

    buffInsertDigit(buff, buffWidth, digitX, digitY, digitWidth, digitHeight, digits[i], config::RED);
  }
}