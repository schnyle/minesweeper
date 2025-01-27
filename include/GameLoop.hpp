#pragma once

#include <Minesweeper.hpp>
#include <Renderer.hpp>

class GameLoop
{
public:
  GameLoop(Minesweeper &, Renderer &);

  void run();

private:
  Minesweeper &game;
  Renderer &renderer;

  bool isRunning = false;

  static const int frameDelayMs = 16; // ~60 fps

  void handleEvents();
  void updateTimer(Uint32 &lastTime, Uint32 &timerAccumulator);
  void render();
  void limitFPS(Uint32 &frameStart);
};