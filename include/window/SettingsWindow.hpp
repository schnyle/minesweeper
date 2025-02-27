#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <config.hpp>
#include <cstdint>
#include <functional>
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
  static SDL_Color hexToRgba(const uint32_t &hexColor);

  TTF_Font *font24 = nullptr;
  TTF_Font *font48 = nullptr;
  bool showConfigWindow = false;

  struct
  {
    SDL_Color black = hexToRgba(config::BLACK);
    SDL_Color grey = hexToRgba(config::GREY);
    SDL_Color darkGrey = hexToRgba(config::DARK_GREY);
  } colors;

  struct SettingsField
  {
    SDL_Rect rect;
    std::string label;
    std::string value;
    uint32_t bgColorHex = config::DARK_GREY;
    bool isEditing = false;

    SettingsField() = default;
    SettingsField(SDL_Rect r, const std::string &l, const std::string v) : rect(r), label(l), value(v) {}
  };

  struct SettingsButton
  {
    SDL_Rect rect;
    std::string label;
    std::function<void()> handleClick;
    uint32_t bgColorHex = config::DARK_GREY;
    bool isPressed = false;

    SettingsButton() = default;
    SettingsButton(SDL_Rect r, const std::string &l, std::function<void()> oc) : rect(r), label(l), handleClick(oc) {}
  };

  struct SettingsMenuFields
  {
    SettingsField cellSize;
    SettingsField windowWidth;
    SettingsField windowHeight;

    std::array<SettingsField *, 3> items() { return {&cellSize, &windowWidth, &windowHeight}; }
  } settingsMenuFields;

  struct SettingsMenuButtons
  {
    SettingsButton save;
    SettingsButton restart;
    SettingsButton defaults;
    SettingsButton cancel;

    std::array<SettingsButton *, 4> items() { return {&save, &restart, &defaults, &cancel}; }
  } settingsMenuButtons;

  void createMenuItems();
  void createMenuButtons();
  void restart();
  bool rectContainsPoint(const SDL_Rect &rect, const int x, const int y);

  void renderContent();
  void renderMenuItem(const SettingsField &menuItem);
  void
  renderTextBox(const std::string &text, const SDL_Color &textColor, const SDL_Color &bgColor, const SDL_Rect &rect);
  void renderText(const std::string text, const SDL_Color &rgba, const int x, const int y);
};