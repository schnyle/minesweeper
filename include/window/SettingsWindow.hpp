#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <string>

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
  TTF_Font *font24 = nullptr;
  TTF_Font *font48 = nullptr;
  std::string setting = "_";
  bool showConfigWindow = false;

  void renderContent();
};