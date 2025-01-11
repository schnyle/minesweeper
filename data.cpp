#include <array>
#include <cstdint>

#include "data.hpp"

uint32_t onePixel[50 * 50];

__attribute__((constructor)) static void initOnePixel()
{
  for (int i = 0; i < 50 * 50; ++i)
  {
    unsigned long c;
    if (!(i * 4 % 3))
    {
      c = 0xffff0000;
    }
    else if (!((i * 4 + 1) % 3))
    {
      c = 0xff00ff00;
    }
    else
    {
      c = 0xff0000ff;
    }

    onePixel[i] = c;
  }
}

Cell data[GRID_HEIGHT][GRID_WIDTH]{
    {
        {false, true, false, 5},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, true, false, 1},
        {false, true, false, 2},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, true, false, 3},
        {false, true, false, 4},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    }};