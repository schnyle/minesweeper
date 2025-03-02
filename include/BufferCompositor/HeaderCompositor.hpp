#pragma once

#include <cstdint>
#include <vector>

#include "BufferCompositor.hpp"
#include "Rect.h"

class HeaderCompositor : public BufferCompositor
{
public:
  static void drawDigit(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n, const int c);

  static void drawHeader(std::vector<uint32_t> &buff, const int width, const int buffSize);
  static void drawRemainingFlags(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n);
  static void drawTimer(std::vector<uint32_t> &buff, const int width, const Rect rect, const int t);
  static void drawGear(std::vector<uint32_t> &buff, const int width, const double center);
};