#pragma once

#include <config.hpp>

#include "BufferCompositor.hpp"

class MinefieldElementCompositor : public BufferCompositor
{
public:
  static const int NUMERIC_SPRITE_HEIGHT = 0.6 * config::CELL_PIXEL_SIZE;
  static const int NUMERIC_SPRITE_WIDTH = NUMERIC_SPRITE_HEIGHT / 2;
  static const int NUMERIC_SPRITE_PAD = (config::CELL_PIXEL_SIZE - NUMERIC_SPRITE_HEIGHT) / 2;

  static void buffInsertMine(uint32_t *buff, const int buffWidth);
  static void buffInsertFlag(uint32_t *buff, const int buffWidth);
  static void buffInsertOne(uint32_t *buff, const int buffWidth);
};