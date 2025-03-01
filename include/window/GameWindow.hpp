#pragma once

#include <Minesweeper.hpp>
#include <SpriteFactory.hpp>
#include <config.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include "Window.hpp"

class GameWindow : public Window
{
public:
  GameWindow();

  void init() override;
  void update(Minesweeper &) override;

private:
  std::vector<uint32_t> frameBuffer;
  std::unique_ptr<SpriteFactory::Sprites> sprites = SpriteFactory::createSprites();

  void updateInterface(Minesweeper &);
  void updateGameArea(Minesweeper &);
};