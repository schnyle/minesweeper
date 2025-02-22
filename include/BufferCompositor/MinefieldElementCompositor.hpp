#pragma once

#include <config.hpp>
#include <vector>

#include "BufferCompositor.hpp"

class MinefieldElementCompositor : public BufferCompositor
{
public:
  static int NUMERIC_SPRITE_HEIGHT;
  static int NUMERIC_SPRITE_WIDTH;
  static int NUMERIC_SPRITE_PAD;

  static void buffInsertMine(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertFlag(std::vector<uint32_t> &buff, const int buffWidth);
  static void buffInsertOne(std::vector<uint32_t> &buff, const int buffWidth);
};
