#pragma once

#include <vector>

#include "BufferCompositor.hpp"

class FaceCompositor : public BufferCompositor
{
public:
  static void buffInsertFaceBase(std::vector<uint32_t> &buff, const int width, const double center);
  static void buffInsertFaceSmile(std::vector<uint32_t> &buff, const int width, const double center);
  static void buffInsertFaceFrown(std::vector<uint32_t> &buff, const int width);
  static void buffInsertFaceAliveEyes(std::vector<uint32_t> &buff, const int width, const double center);
  static void buffInsertFaceDeadEyes(std::vector<uint32_t> &buff, const int width);
  static void buffInsertFaceShades(std::vector<uint32_t> &buff, const int width);
  static void buffInsertX(
      std::vector<uint32_t> &buff,
      const int width,
      const uint32_t color,
      const double xCenter,
      const double yCenter,
      const double xSize,
      const double xThickness);
};