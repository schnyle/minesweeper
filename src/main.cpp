#include <GameLoop.hpp>
#include <Minesweeper.hpp>
#include <Renderer.hpp>
#include <sprites.hpp>

int main(int, char **)
{
  if (!Renderer::initSDL())
  {
    return 1;
  };

  sprites::init();

  Minesweeper game;
  Renderer renderer;
  GameLoop gameLoop(game, renderer);
  gameLoop.run();

  return 0;
}
