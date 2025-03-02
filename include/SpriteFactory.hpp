#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "config.hpp"

class SpriteFactory
{
public:
  int resetButtonSize = config::INFO_PANEL_BUTTONS_HEIGHT * config::INFO_PANEL_BUTTONS_HEIGHT;
  int cellSpriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE;
  struct Sprites
  {
    std::vector<uint32_t> raisedResetButton;
    std::vector<uint32_t> pressedResetButton;
    std::vector<uint32_t> winnerResetButton;
    std::vector<uint32_t> loserResetButton;
    std::vector<uint32_t> raisedConfigButton;
    std::vector<uint32_t> pressedConfigButton;
    std::vector<uint32_t> empty;
    std::vector<uint32_t> hidden;
    std::vector<uint32_t> flag;
    std::vector<uint32_t> mine;
    std::vector<uint32_t> clickedMine;
    std::vector<uint32_t> redXMine;
    std::vector<uint32_t> zero;
    std::vector<uint32_t> one;
    std::vector<uint32_t> two;
    std::vector<uint32_t> three;
    std::vector<uint32_t> four;
    std::vector<uint32_t> five;
    std::vector<uint32_t> six;
    std::vector<uint32_t> seven;
    std::vector<uint32_t> eight;
    std::map<int, std::vector<uint32_t>> intToSpriteMap;
  };

  SpriteFactory(Sprites *spriteObjs);

  static std::unique_ptr<Sprites> createSprites();
  static void copySprite(
      std::vector<uint32_t> &buff,
      const std::vector<uint32_t> &sprite,
      const int spriteWidth,
      const int x,
      const int y);

private:
  Sprites *sprites;
};
