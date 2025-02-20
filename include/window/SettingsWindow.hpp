#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

#include "Window.hpp"

class SettingsWindow : public Window
{
public:
  SettingsWindow();
  ~SettingsWindow();

  void init() override;
  void update(Minesweeper &) override;
  void handleEvents(SDL_Event &event);

private:
  TTF_Font *font24;
  TTF_Font *font48;
  std::string setting = "_";
  bool showConfigWindow = false;

  void renderContent();
};