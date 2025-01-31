#pragma once

#include <cstdint>

#include "Window.hpp"

class SettingsWindow : public Window
{
public:
  SettingsWindow();
  ~SettingsWindow();

  void init(uint32_t *frameBuffer) override;
  void updateFrameBuffer(Minesweeper &) override;

  void updateConfigWindow(Minesweeper &);

private:
  TTF_Font *font24;
  TTF_Font *font48;

  bool showConfigWindow = false;
};