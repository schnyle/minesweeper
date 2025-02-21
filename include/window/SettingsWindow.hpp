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
  struct
  {
    SDL_Color black = hexToRgba(config::BLACK);
    SDL_Color grey = hexToRgba(config::GREY);
    SDL_Color darkGrey = hexToRgba(config::DARK_GREY);
  } colors;

  struct MenuItem
  {
    SDL_Rect rect;
    std::string label;
    std::string value;
    uint32_t bgColorHex = config::DARK_GREY;
    bool isEditing = false;

    MenuItem(SDL_Rect r, const std::string l, const std::string v) : rect(r), label(l), value(v) {}
  };

  struct
  {
    SDL_Rect rect{15, 200, 200, 40};
    uint32_t bgColorHex = config::DARK_GREY;
    bool isPressed = false;
  } restartButton;

  TTF_Font *font24 = nullptr;
  TTF_Font *font48 = nullptr;
  bool showConfigWindow = false;
  std::vector<MenuItem> settingsMenuItems;

  void createMenuItems();
  void restart();
  bool rectContainsPoint(const SDL_Rect &rect, const int x, const int y);
  static SDL_Color hexToRgba(const uint32_t &hexColor);

  void renderContent();
  void renderMenuItem(const MenuItem &menuItem);
  void
  renderTextBox(const std::string &text, const SDL_Color &textColor, const SDL_Color &bgColor, const SDL_Rect &rect);
  void renderText(const std::string text, const SDL_Color &rgba, const int x, const int y);
};