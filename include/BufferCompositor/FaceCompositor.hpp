#pragma once

#include <vector>

#include "BufferCompositor.hpp"

class FaceCompositor : public BufferCompositor
{
public:
  static void buffInsertFaceBase(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertFaceSmile(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertFaceFrown(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertFaceAliveEyes(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertFaceDeadEyes(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertFaceShades(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertX(
      std::vector<uint32_t> &buff,
      const int buffWidth,
      const uint32_t color,
      const double xCenter,
      const double yCenter,
      const double xSize,
      const double xThickness);
};