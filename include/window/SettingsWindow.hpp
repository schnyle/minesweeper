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

    bool contains(const int x, const int y) const
    {
      return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
    }
  };

  TTF_Font *font24 = nullptr;
  TTF_Font *font48 = nullptr;
  bool showConfigWindow = false;
  std::vector<MenuItem> settingsMenuItems;

  void createMenuItems();

  void renderContent();
  void renderMenuItem(const MenuItem &menuItem);
  void renderText(const std::string text, const SDL_Color &rgba, const int x, const int y);
  static SDL_Color hexToRgba(const uint32_t &hexColor);
};