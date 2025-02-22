#pragma once

#include <cstdint>
#include <vector>

#include "BufferCompositor.hpp"

class HeaderCompositor : public BufferCompositor
{
public:
  static void buffInsertHeader(std::vector<uint32_t> &buff, const int buffWidth, const int buffSize);
  static void buffInsertRemainingFlags(
      std::vector<uint32_t> &buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const int n);
  static void buffInsertTimer(
      std::vector<uint32_t> &buff,
      const int buffWidth,
      const int x,
      const int y,
      const int w,
      const int h,
      const int t);
};