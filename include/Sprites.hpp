#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

class Sprites
{
public:
  struct SpriteData
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

  static Sprites &getInstance();
  static void
  copy(const std::vector<uint32_t> &source, std::vector<uint32_t> &target, const int width, const int x, const int y);

  const SpriteData *get();

private:
  Sprites();

  Sprites(const Sprites &) = delete;
  Sprites &operator=(const Sprites &) = delete;
  Sprites(Sprites &&) = delete;
  Sprites &operator=(Sprites &&) = delete;

  bool isInitialized = false;
  std::unique_ptr<SpriteData> data = std::make_unique<SpriteData>();

  void allocateMemory();
  void drawSprites();
  void createIntToSpriteMap();
};
