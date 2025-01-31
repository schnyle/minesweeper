#pragma once

#include <Minesweeper.hpp>
#include <cstdint>
#include <memory>

#include "Window.hpp"

class GameWindow : public Window
{
public:
  GameWindow();

  void init() override;
  void update(Minesweeper &) override;

private:
  std::unique_ptr<uint32_t[]> frameBuffer;

  void updateInterface(Minesweeper &);
  void updateGameArea(Minesweeper &);
};