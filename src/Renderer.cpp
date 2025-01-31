#include <Minesweeper.hpp>
#include <Renderer.hpp>
#include <SpriteFactory.hpp>
#include <algorithm>
#include <chrono>
#include <config.hpp>
#include <cstring>
#include <iostream>
#include <memory>

#include <SDL.h>
#include <SDL_ttf.h>

Renderer::Renderer()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "error initializing SDL: " << SDL_GetError() << std::endl;
  }

  frameBuffer = std::make_unique<uint32_t[]>(WIDTH * HEIGHT);
  if (!frameBuffer)
  {
    std::cout << "error allocating frame buffer" << std::endl;
  }

  SpriteFactory::buffInsertInterface(frameBuffer.get(), WIDTH, WIDTH * HEIGHT);

  sprites = SpriteFactory::createSprites();

  gameWindow.init(frameBuffer.get());
}

Renderer::~Renderer() { SDL_Quit(); }

void Renderer::renderFrame()
{
  void *pixels;
  int pitch;

  SDL_LockTexture(gameWindow.getTexture(), nullptr, &pixels, &pitch);
  std::memcpy(pixels, frameBuffer.get(), WIDTH * HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(gameWindow.getTexture());
  SDL_RenderCopy(gameWindow.getRenderer(), gameWindow.getTexture(), nullptr, nullptr);
  SDL_RenderPresent(gameWindow.getRenderer());
}
