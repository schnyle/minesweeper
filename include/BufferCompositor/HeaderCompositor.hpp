#pragma once

#include <cstdint>

#include "BufferCompositor.hpp"

class HeaderCompositor : public BufferCompositor
{
public:
  static void buffInsertHeader(uint32_t *buff, const int buffWidth, const int buffSize);
  static void buffInsertRemainingFlags(
      uint32_t *buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const int n);
  static void
  buffInsertTimer(uint32_t *buff, const int buffWidth, const int x, const int y, const int w, const int h, const int t);
};