#pragma once

#include <vector>

#include "BaseArtist.hpp"

class FaceArtist : public BaseArtist
{
public:
  static void drawFaceBase(std::vector<uint32_t> &buff, const int width, const double center);
  static void drawFaceSmile(std::vector<uint32_t> &buff, const int width, const double center);
  static void drawFaceFrown(std::vector<uint32_t> &buff, const int width);
  static void drawFaceAliveEyes(std::vector<uint32_t> &buff, const int width, const double center);
  static void drawFaceDeadEye(std::vector<uint32_t> &buff, const int width);
  static void drawFaceShade(std::vector<uint32_t> &buff, const int width);
  static void drawX(
      std::vector<uint32_t> &buff,
      const int width,
      const uint32_t color,
      const double xCenter,
      const double yCenter,
      const double xSize,
      const double xThickness);
};