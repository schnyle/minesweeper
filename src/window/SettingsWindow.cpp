#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SettingsWindow.hpp>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <utils.hpp>

#include "font.h"

SettingsWindow::SettingsWindow() : Window()
{
  if (TTF_Init() < 0)
  {
    throw std::runtime_error(std::string("error initializing SDL TTF: ") + TTF_GetError());
  }

  SDL_RWops *rw = SDL_RWFromMem(assets_UbuntuMono_B_ttf, assets_UbuntuMono_B_ttf_len);
  font24.reset(TTF_OpenFontRW(rw, 0, 24));
  font48.reset(TTF_OpenFontRW(rw, 0, 48));
  if (!font24 || !font48)
  {
    throw std::runtime_error(std::string("failed to load font: ") + TTF_GetError());
  }
}

SettingsWindow::~SettingsWindow()
{
  font24.reset(nullptr);
  font48.reset(nullptr);
  TTF_Quit();
}

void SettingsWindow::init()
{
  pixelWidth = config::CONFIG_WINDOW_PIXEL_WIDTH;
  pixelHeight = config::CONFIG_WINDOW_PIXEL_HEIGHT;

  createMenuItems();
  createMenuButtons();

  window.reset(SDL_CreateWindow(
      "Minesweeper Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pixelWidth, pixelHeight, SDL_WINDOW_SHOWN));
  if (window == nullptr)
  {
    std::cerr << "error creating window: " << SDL_GetError() << std::endl;
  }

  renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  if (renderer == nullptr)
  {
    std::cerr << "error getting config renderer: " << SDL_GetError() << std::endl;
  }

  windowID = SDL_GetWindowID(window.get());
}

void SettingsWindow::update(Minesweeper &game)
{
  bool newShowConfig = game.getShowConfigButton();

  if (showConfigWindow != newShowConfig)
  {
    showConfigWindow = newShowConfig;
    if (showConfigWindow)
    {
      if (window == nullptr)
      {
        init();
      }

      SDL_ShowWindow(window.get());
      renderContent();
    }
    else
    {
      SDL_HideWindow(window.get());
    }
  }
  else if (showConfigWindow)
  {
    renderContent();
  }
}

void SettingsWindow::handleEvents(SDL_Event &event)
{
  const int cursorX = event.motion.x;
  const int cursorY = event.motion.y;

  switch (event.type)
  {
  case SDL_MOUSEBUTTONDOWN:
    for (auto *button : settingsMenuButtons.items())
    {
      if (utils::isPointInRect(cursorX, cursorY, button->rect))
      {
        button->isPressed = true;
        button->bgColorHex = config::LIGHT_BLUE;
      }
    }

    for (auto *menuItem : settingsMenuFields.items())
    {
      if (utils::isPointInRect(cursorX, cursorY, menuItem->rect))
      {
        menuItem->bgColorHex = config::LIGHT_BLUE;
        menuItem->isEditing = true;
      }
      else
      {
        menuItem->bgColorHex = config::DARK_GREY;
        menuItem->isEditing = false;
      }
    }

    break;

  case SDL_MOUSEBUTTONUP:
    for (auto *button : settingsMenuButtons.items())
    {
      if (button->isPressed && utils::isPointInRect(cursorX, cursorY, button->rect))
      {
        button->handleClick();
      }
      button->isPressed = false;
      button->bgColorHex = config::DARK_GREY;
    }
    break;

  case SDL_KEYDOWN:
  {
    std::string *value = nullptr;
    for (auto *menuItem : settingsMenuFields.items())
    {
      if (menuItem->isEditing)
      {
        value = &menuItem->value;
      }
    }

    if (value == nullptr)
    {
      return;
    }

    const auto keycode = event.key.keysym.sym;
    if (keycode == SDLK_BACKSPACE && value->size() > 0)
    {
      value->pop_back();
    }
    else if (keycode >= SDLK_0 && keycode <= SDLK_9)
    {
      *value += keycode;
    }
  }
  }
};

void SettingsWindow::createMenuItems()
{
  const int leftPad = 15;
  const int topPad = 15;
  const int ySep = 50;
  const int menuItemRectWidth = config::CONFIG_WINDOW_PIXEL_WIDTH * 0.75;
  const int menuItemRectHeight = 40;

  settingsMenuFields.cellSize.rect = SDL_Rect{leftPad, topPad + 0 * ySep, menuItemRectWidth, menuItemRectHeight};
  settingsMenuFields.cellSize.label = "Cell Size";
  settingsMenuFields.cellSize.value = std::to_string(config::CELL_PIXEL_SIZE);

  settingsMenuFields.windowWidth.rect = SDL_Rect{leftPad, topPad + 1 * ySep, menuItemRectWidth, menuItemRectHeight};
  settingsMenuFields.windowWidth.label = "Window Width";
  settingsMenuFields.windowWidth.value = std::to_string(config::GAME_WINDOW_PIXEL_WIDTH);

  settingsMenuFields.windowHeight.rect = SDL_Rect{leftPad, topPad + 2 * ySep, menuItemRectWidth, menuItemRectHeight};
  settingsMenuFields.windowHeight.label = "Window Height";
  settingsMenuFields.windowHeight.value = std::to_string(config::GAME_WINDOW_PIXEL_HEIGHT);
}

void SettingsWindow::createMenuButtons()
{
  settingsMenuButtons.save.rect = SDL_Rect{15, 250, 200, 40};
  settingsMenuButtons.save.label = "SAVE";
  settingsMenuButtons.save.handleClick = [&]()
  {
    const int cellSize = std::stoi(settingsMenuFields.cellSize.value);
    const int windowW = std::stoi(settingsMenuFields.windowWidth.value);
    const int windowH = std::stoi(settingsMenuFields.windowHeight.value);
    config::writeConfigFile(windowW, windowH, cellSize);
  };

  settingsMenuButtons.restart.rect = SDL_Rect{15, 300, 200, 40};
  settingsMenuButtons.restart.label = "RESTART";
  settingsMenuButtons.restart.handleClick = [&]()
  {
    char *basePath = SDL_GetBasePath();
    if (!basePath)
    {
      throw std::runtime_error("Couldn't get program path");
    }

    std::string programPath = std::string(basePath) + "minesweeper";
    SDL_free(basePath);

    char *argv[] = {programPath.data(), nullptr};
    execv(argv[0], argv);

    throw std::runtime_error("Failed to restart program");
  };

  settingsMenuButtons.defaults.rect = SDL_Rect{15, 350, 200, 40};
  settingsMenuButtons.defaults.label = "DEFAULT";
  settingsMenuButtons.defaults.handleClick = [&]()
  {
    const int defaultWidth = config::DISPLAY_PIXEL_WIDTH * config::GAME_WINDOW_TO_DISPLAY_RATIO;
    const int defaultHeight = config::DISPLAY_PIXEL_HEIGHT * config::GAME_WINDOW_TO_DISPLAY_RATIO;

    settingsMenuFields.cellSize.value = std::to_string(config::DEFAULT_CELL_PIXEL_SIZE);
    settingsMenuFields.windowWidth.value = std::to_string(defaultWidth);
    settingsMenuFields.windowHeight.value = std::to_string(defaultHeight);
  };

  settingsMenuButtons.cancel.rect = SDL_Rect{15, 400, 200, 40};
  settingsMenuButtons.cancel.label = "CANCEL";
  settingsMenuButtons.cancel.handleClick = [&]() { createMenuItems(); };
}

void SettingsWindow::restart()
{
  char *basePath = SDL_GetBasePath();
  if (!basePath)
  {
    throw std::runtime_error("Couldn't get program path");
  }

  std::string programPath = std::string(basePath) + "minesweeper";
  SDL_free(basePath);

  char *argv[] = {programPath.data(), nullptr};
  execv(argv[0], argv);

  throw std::runtime_error("Failed to restart program");
}

void SettingsWindow::renderContent()
{
  if (SDL_SetRenderDrawColor(renderer.get(), colors.grey.r, colors.grey.g, colors.grey.b, colors.grey.a) < 0)
  {
    std::cerr << "error setting render draw color: " << SDL_GetError() << std::endl;
    return;
  }

  if (SDL_RenderClear(renderer.get()) < 0)
  {
    std::cerr << "error calling RenderClear: " << SDL_GetError() << std::endl;
    return;
  }

  for (const auto *menuItem : settingsMenuFields.items())
  {
    renderMenuItem(*menuItem);
  }
  for (const auto *button : settingsMenuButtons.items())
  {
    renderTextBox(button->label, colors.black, utils::hexToRgba(button->bgColorHex), button->rect);
  }

  SDL_RenderPresent(renderer.get());
}

void SettingsWindow::renderMenuItem(const SettingsField &menuItem)
{
  const auto menuItemRect = menuItem.rect;
  const auto bgColorHex = menuItem.bgColorHex;
  const SDL_Color bgColorRGBA = utils::hexToRgba(bgColorHex);
  const std::string text = menuItem.label + ": " + menuItem.value;

  renderTextBox(text, colors.black, bgColorRGBA, menuItemRect);
}

void SettingsWindow::renderTextBox(
    const std::string &text,
    const SDL_Color &textColor,
    const SDL_Color &bgColor,
    const SDL_Rect &rect)
{
  SDL_SetRenderDrawColor(renderer.get(), bgColor.r, bgColor.g, bgColor.b, bgColor.a);
  SDL_RenderFillRect(renderer.get(), &rect);

  SDL_SetRenderDrawColor(renderer.get(), textColor.r, textColor.g, textColor.b, textColor.a);
  SDL_RenderDrawRect(renderer.get(), &rect);

  renderText(text, colors.black, rect.x, rect.y);
};

void SettingsWindow::renderText(const std::string text, const SDL_Color &rgba, const int x, const int y)
{
  if (text.size() == 0)
  {
    return;
  }

  SDL_Surface *textSurface = TTF_RenderText_Solid(font24.get(), text.c_str(), {rgba.r, rgba.g, rgba.b, rgba.a});
  if (textSurface == nullptr)
  {
    std::cerr << "error creating text surface: " << TTF_GetError() << std::endl;
    return;
  }

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer.get(), textSurface);
  if (textTexture == nullptr)
  {
    std::cerr << "error creating text texture: " << SDL_GetError() << std::endl;
    SDL_FreeSurface(textSurface);
    return;
  }

  SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
  SDL_RenderCopy(renderer.get(), textTexture, nullptr, &textRect);

  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
};