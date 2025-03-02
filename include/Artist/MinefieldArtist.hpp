#pragma once

#include <Minesweeper.hpp>
#include <config.hpp>
#include <cstdint>
#include <vector>

#include "BaseArtist.hpp"

class MinefieldArtist : public BaseArtist
{
public:
  static void updateMinefield(std::vector<uint32_t> &buff, const int width, Minesweeper &gameState);

  static void drawEmptyCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawHiddenCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawFlaggedCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawMineCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawMineCellRedXSprite(std::vector<uint32_t> &buff, const int width);
  static void drawClickedMineCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawNumericSprite(std::vector<uint32_t> &buff, const int width, const int n, const uint32_t c);

private:
  static int NUMERIC_SPRITE_HEIGHT;
  static int NUMERIC_SPRITE_WIDTH;
  static int NUMERIC_SPRITE_PAD;

  static void drawMine(std::vector<uint32_t> &buff, const int width);
  static void drawFlag(std::vector<uint32_t> &buff, const int width);
  static void drawOne(std::vector<uint32_t> &buff, const int width);

  static const std::vector<uint32_t> &getCellSprite(const Minesweeper &gameState, const int cellIndex);
};
