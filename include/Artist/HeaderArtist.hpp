#pragma once

#include <Minesweeper.hpp>
#include <cstdint>
#include <vector>

#include "BaseArtist.hpp"
#include "Rect.h"

class HeaderArtist : public BaseArtist
{
public:
  static void drawHeader(std::vector<uint32_t> &buff, const int width, const int buffSize);
  static void updateHeader(std::vector<uint32_t> &buff, const int width, const Minesweeper &gameState);

  static void drawRaisedResetButtonSprite(std::vector<uint32_t> &buff, const int width);
  static void drawPressedResetButtonSprite(std::vector<uint32_t> &buff, const int width);
  static void drawWinnerResetButtonSprite(std::vector<uint32_t> &buff, const int width);
  static void drawLoserResetButtonSprite(std::vector<uint32_t> &buff, const int width);
  static void drawRaisedConfigButtonSprite(std::vector<uint32_t> &buff, const int width);
  static void drawPressedConfigButtonSprite(std::vector<uint32_t> &buff, const int width);

private:
  static void drawTriDigit(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n);
  static void drawGear(std::vector<uint32_t> &buff, const int width, double center = -1);

  static const std::vector<uint32_t> &getResetButtonSprite(const Minesweeper &gameState);
  static const std::vector<uint32_t> &getConfigButtonSprite(const Minesweeper &gameState);
};