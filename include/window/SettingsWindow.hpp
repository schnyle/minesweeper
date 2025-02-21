#pragma once

#include <SDL2/SDL.h>
#include <config.hpp>
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
  std::string setting = std::to_string(config::GAME_WINDOW_PIXEL_WIDTH);
  bool showConfigWindow = false;

  struct
  {
    SDL_Color black = hexToRgba(config::BLACK);
    SDL_Color grey = hexToRgba(config::GREY);
  } colors;

  void renderContent();
  void renderText(const std::string text, const SDL_Color &rgba, const int x, const int y);
  static SDL_Color hexToRgba(const uint32_t &hexColor);
};