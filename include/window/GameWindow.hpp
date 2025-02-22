#pragma once

#include <Minesweeper.hpp>
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

  void updateInterface(Minesweeper &);
  void updateGameArea(Minesweeper &);
};