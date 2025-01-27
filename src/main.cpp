#include <GameLoop.hpp>
#include <Minesweeper.hpp>
#include <Renderer.hpp>

int main()
{
  Minesweeper game;
  Renderer renderer;
  GameLoop gameLoop(game, renderer);
  gameLoop.run();
  return 0;
}