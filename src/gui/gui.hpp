#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "engine/engine.hpp"
#include "gui/layout.hpp"

class Gui
{
public:
  Gui(WindowSizePixels window_size, LayoutStyle layout_style, const Engine &engine);
  ~Gui();

  Gui(const Gui &) = delete;
  Gui &operator=(const Gui &) = delete;
  Gui(Gui &&) = delete;
  Gui &operator=(Gui &&) = delete;

  [[nodiscard]] const Layout &layout() const { return layout_; };

  void render(const Engine &);

private:
  Layout layout_;
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
};
