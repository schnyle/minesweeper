#include <Game.hpp>
#include <RasterRenderer.hpp>
#include <Renderer.hpp>

int main()
{
  Game game;
  // Renderer renderer;
  RasterRenderer renderer;
  renderer.run(game);
  return 0;
}