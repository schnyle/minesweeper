#pragma once

#include <config.hpp>
#include <cstdint>
#include <vector>

#include "Rect.h"

class BufferCompositor
{
  // protected:
public: // make protected
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

  static void buffInsertRectangle(std::vector<uint32_t> &buff, const int width, const Rect rect, const uint32_t c);
  static void buffInsert2DBorder(std::vector<uint32_t> &buff, const int width, const Rect rect, const uint32_t c);
  static void buffInsert3DBorder(
      std::vector<uint32_t> &buff,
      const int width,
      const Rect rect,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void buffInsert3DCorner(
      std::vector<uint32_t> &buff,
      const int width,
      const Rect rect,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void buffInsertDigit(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n, const int c);
};