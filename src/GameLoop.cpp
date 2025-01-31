#include <GameLoop.hpp>
#include <SDL.h>

GameLoop::GameLoop(Minesweeper &g, Renderer &r) : game(g), renderer(r) {}

void GameLoop::run()
{
  isRunning = true;

  Uint32 lastTime = SDL_GetTicks();
  Uint32 timerAccumulator = 0;

  while (isRunning)
  {
    auto frameStart = SDL_GetTicks();

    handleEvents();
    updateTimer(lastTime, timerAccumulator);
    render();
    limitFPS(frameStart);
  }
}

void GameLoop::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      isRunning = false;
    }

    if (event.window.windowID == renderer.gameWindow.getWindowID())
    {
      if (!game.updateGameState(event))
      {
        isRunning = false;
      }
    }

    if (event.window.windowID == renderer.configWindowID)
    {
    }
  }
}

void GameLoop::updateTimer(Uint32 &lastTime, Uint32 &timerAccumulator)
{
  static const int oneSecondMs = 1000;

  auto currentTime = SDL_GetTicks();
  timerAccumulator += currentTime - lastTime;
  while (timerAccumulator >= oneSecondMs)
  {
    game.incrementTimer();
    timerAccumulator -= oneSecondMs;
  }
  lastTime = currentTime;
}

void GameLoop::render()
{
  renderer.gameWindow.updateFrameBuffer(game);
  renderer.settingsWindow.updateConfigWindow(game);
  renderer.renderFrame();
}

void GameLoop::limitFPS(Uint32 &frameStart)
{
  auto frameTicks = SDL_GetTicks() - frameStart;
  if (frameTicks < frameDelayMs)
  {
    SDL_Delay(frameDelayMs - frameTicks);
  }
}