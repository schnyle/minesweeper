#pragma once

#include <cstdint>
#include <vector>

#include "BufferCompositor.hpp"
#include "Rect.h"

class HeaderCompositor : public BufferCompositor
{
public:
  static void buffInsertHeader(std::vector<uint32_t> &buff, const int width, const int buffSize);
  static void buffInsertRemainingFlags(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n);
  static void buffInsertTimer(std::vector<uint32_t> &buff, const int width, const Rect rect, const int t);
  static void buffInsertGear(std::vector<uint32_t> &buff, const int width, const double center);
};