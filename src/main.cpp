#include <GameLoop.hpp>
#include <Minesweeper.hpp>
#include <Renderer.hpp>

int main(int, char **)
{
  if (!Renderer::initSDL())
  {
    return 1;
  };

  Minesweeper game;
  Renderer renderer;
  GameLoop gameLoop(game, renderer);
  gameLoop.run();

  return 0;
}