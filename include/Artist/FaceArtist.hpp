#pragma once

#include <vector>

#include "BaseArtist.hpp"

class FaceArtist : public BaseArtist
{
public:
  static void drawFaceBase(std::vector<uint32_t> &buff, const int width, double center = -1);
  static void drawFaceSmile(std::vector<uint32_t> &buff, const int width, double center = -1);
  static void drawFaceFrown(std::vector<uint32_t> &buff, const int width);
  static void drawFaceAliveEyes(std::vector<uint32_t> &buff, const int width, double center = -1);
  static void drawFaceDeadEye(std::vector<uint32_t> &buff, const int width);
  static void drawFaceShade(std::vector<uint32_t> &buff, const int width);
};