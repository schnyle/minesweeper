#pragma once

#include <config.hpp>
#include <cstdint>

class BufferCompositor
{
  // protected:
public: // make protected
  static const int NUMERIC_SPRITE_HEIGHT = 0.6 * config::CELL_PIXEL_SIZE;
  struct DigitSegments
  {
    bool topMiddle;
    bool topLeft;
    bool topRight;
    bool middleMiddle;
    bool bottomMiddle;
    bool bottomLeft;
    bool bottomRight;
  };
  static DigitSegments intToDigitSegments(const int n);

  static void buffInsertRectangle(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t c);
  static void buffInsert2DBorder(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t c);
  static void buffInsert3DBorder(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void buffInsert3DCorner(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void buffInsertDigit(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const int n,
      const int c);
};