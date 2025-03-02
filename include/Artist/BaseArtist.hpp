#pragma once

#include <config.hpp>
#include <cstdint>
#include <vector>

#include "Rect.h"

class BaseArtist
{
protected:
  static void draw2DCellBase(std::vector<uint32_t> &buff, const int width);
  static void draw3DCellBase(std::vector<uint32_t> &buff, const int width);
  static void draw2DBorder(std::vector<uint32_t> &buff, const int width, const Rect rect, const uint32_t c);
  static void draw3DBorder(
      std::vector<uint32_t> &buff,
      const int width,
      const Rect rect,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);
  static void draw3DCorner(
      std::vector<uint32_t> &buff,
      const int width,
      const Rect rect,
      const uint32_t cTop,
      const uint32_t cMid,
      const uint32_t cBot);

  static void drawRectangle(std::vector<uint32_t> &buff, const int width, const Rect rect, const uint32_t c);
  static void drawDigit(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n, const int c);
  static void drawX(
      std::vector<uint32_t> &buff,
      const int width,
      const uint32_t c,
      const double xCenter,
      const double yCenter,
      const double xSize,
      const double xThickness);

private:
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
};