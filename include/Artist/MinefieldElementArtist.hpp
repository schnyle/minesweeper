#pragma once

#include <config.hpp>
#include <vector>

#include "BaseArtist.hpp"

class MinefieldElementArtist : public BaseArtist
{
public:
  static int NUMERIC_SPRITE_HEIGHT;
  static int NUMERIC_SPRITE_WIDTH;
  static int NUMERIC_SPRITE_PAD;

  static void drawMineCell(std::vector<uint32_t> &buff, const int width);
  static void drawClickedMineCell(std::vector<uint32_t> &buff, const int width);

  static void draw2DCellBase(std::vector<uint32_t> &buff, const int width);
  static void draw3DCellBase(std::vector<uint32_t> &buff, const int width);

  static void drawMine(std::vector<uint32_t> &buff, const int width);
  static void drawFlag(std::vector<uint32_t> &buff, const int width);
  static void drawOne(std::vector<uint32_t> &buff, const int width);
};
