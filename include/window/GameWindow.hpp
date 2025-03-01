#pragma once

#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
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
  ~GameWindow() override = default;

  void init() override;
  void update(Minesweeper &) override;
  void handleEvent(SDL_Event &event, Minesweeper &gameState, bool &isGameLoopRunning);

private:
  const int gameAreaX = config::GRID_AREA_X_PAD + config::FRAME_WIDTH;
  const int gameAreaY = config::GRID_AREA_Y_PAD + config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;
  std::unique_ptr<SpriteFactory::Sprites> sprites = SpriteFactory::createSprites();

  void updateInterface(const Minesweeper &gameState);
  void updateFrameBuffer(const Minesweeper &gameState);

  const std::vector<uint32_t> &getResetButtonSprite(const Minesweeper &gameState) const;
  const std::vector<uint32_t> &getConfigButtonSprite(const Minesweeper &gameState) const;
  const std::vector<uint32_t> &getCellSprite(const Minesweeper &gameState, const int cellIndex) const;
};