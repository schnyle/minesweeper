#include <Game.hpp>
#include <RasterRenderer.hpp>
#include <Renderer.hpp>
#include <string>

int main(int argc, char *argv[])
{
  // temporary
  bool useRasterGraphics = false;
  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      std::string arg = argv[i];

      if (arg[0] == '-')
      {
        if (arg[1] == 'r')
        {
          useRasterGraphics = true;
        }
      }
    }
  }

  Game game;

  if (useRasterGraphics)
  {
    RasterRenderer renderer;
    renderer.run(game);
  }
  else
  {
    Renderer renderer;
    renderer.run(game);
  }

  return 0;
}