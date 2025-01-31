#pragma once

#include <cstdint>

#include "Window.hpp"

class SettingsWindow : public Window
{
public:
  SettingsWindow();

  void init() override;
  void update(Minesweeper &) override;

private:
  TTF_Font *font24;
  TTF_Font *font48;

  bool showConfigWindow = false;
};