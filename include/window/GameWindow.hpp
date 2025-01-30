#pragma once

#include <Minesweeper.hpp>
#include <cstdint>

#include "Window.hpp"

class GameWindow : public Window
{
public:
  ~GameWindow();

  void init(uint32_t *frameBuffer) override;
  void updateFrameBuffer(Minesweeper &) override;

  void updateInterface(Minesweeper &);
  void updateGameArea(Minesweeper &);
};