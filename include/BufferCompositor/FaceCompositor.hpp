#pragma once

#include "BufferCompositor.hpp"

class FaceCompositor : public BufferCompositor
{
public:
  static void buffInsertFaceBase(uint32_t *buff, const int buffWidth);
  static void buffInsertFaceSmile(uint32_t *buff, const int buffWidth);
  static void buffInsertFaceFrown(uint32_t *buff, const int buffWidth);
  static void buffInsertFaceAliveEyes(uint32_t *buff, const int buffWidth);
  static void buffInsertFaceDeadEyes(uint32_t *buff, const int buffWidth);
  static void buffInsertFaceShades(uint32_t *buff, const int buffWidth);
  static void buffInsertX(
      uint32_t *buff,
      const int buffWidth,
      const uint32_t color,
      const double xCenter,
      const double yCenter,
      const double xSize,
      const double xThickness);
};