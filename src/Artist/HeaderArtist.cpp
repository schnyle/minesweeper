#include <FaceArtist.hpp>
#include <HeaderArtist.hpp>
#include <MinefieldArtist.hpp>
#include <Sprites.hpp>
#include <algorithm>
#include <config.hpp>
#include <cstdint>
#include <vector>

#include "Rect.h"

// public

void HeaderArtist::drawHeader(std::vector<uint32_t> &buff, const int width, const int buffSize)
{
  // base
  std::fill_n(buff.begin(), buffSize, config::Colors::GREY);

  // border
  BaseArtist::draw3DBorder(
      buff,
      width,
      {0, 0, config::getSettings().getGameWindowWidth(), config::getSettings().getGameWindowHeight()},
      config::Colors::LIGHT_GREY,
      config::Colors::GREY,
      config::Colors::DARK_GREY);

  BaseArtist::draw3DBorder(
      buff,
      width,
      {config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D(),
       config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D(),
       config::getSettings().getGameWindowWidth() -
           2 * (config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D()),
       config::getSettings().getGameWindowHeight() -
           2 * (config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D())},
      config::Colors::DARK_GREY,
      config::Colors::GREY,
      config::Colors::LIGHT_GREY);

  // header / game separator
  BaseArtist::draw3DBorder(
      buff,
      width,
      {config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D(),
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
       config::getSettings().getGameWindowWidth() -
           2 * (config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D()),
       config::FRAME_WIDTH},
      config::Colors::LIGHT_GREY,
      config::Colors::GREY,
      config::Colors::DARK_GREY);
  // fill overlap
  BaseArtist::drawRectangle(
      buff,
      width,
      {config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D(),
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT + config::getSettings().getCellBorderWidth3D(),
       config::getSettings().getGameWindowWidth() -
           2 * (config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D()),
       config::FRAME_WIDTH - 2 * config::getSettings().getCellBorderWidth3D()},
      config::Colors::GREY);
  // top-left corner
  BaseArtist::draw3DCorner(
      buff,
      width,
      {config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D(),
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
       config::getSettings().getCellBorderWidth3D(),
       config::getSettings().getCellBorderWidth3D()},
      config::Colors::DARK_GREY,
      config::Colors::GREY,
      config::Colors::LIGHT_GREY);
  // top-right corner
  BaseArtist::drawRectangle(
      buff,
      width,
      {config::getSettings().getGameWindowWidth() - config::FRAME_WIDTH,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT,
       config::getSettings().getCellBorderWidth3D(),
       config::getSettings().getCellBorderWidth3D()},
      config::Colors::LIGHT_GREY);
  // bottom-left corner
  BaseArtist::drawRectangle(
      buff,
      width,
      {config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D(),
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT +
           (config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D()),
       config::getSettings().getCellBorderWidth3D(),
       config::getSettings().getCellBorderWidth3D()},
      config::Colors::DARK_GREY);
  // bottom-right corner
  BaseArtist::draw3DCorner(
      buff,
      width,
      {config::getSettings().getGameWindowWidth() - config::FRAME_WIDTH,
       config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT +
           (config::FRAME_WIDTH - config::getSettings().getCellBorderWidth3D()),
       config::getSettings().getCellBorderWidth3D(),
       config::getSettings().getCellBorderWidth3D()},
      config::Colors::DARK_GREY,
      config::Colors::GREY,
      config::Colors::LIGHT_GREY);
};

void HeaderArtist::updateHeader(std::vector<uint32_t> &buff, const int width, const Minesweeper &gameState)
{
  drawTriDigit(
      buff,
      width,
      {config::getSettings().getRemainingFlagsX(),
       config::getSettings().getRemainingFlagsY(),
       config::INFO_PANEL_BUTTONS_HEIGHT * 2,
       config::INFO_PANEL_BUTTONS_HEIGHT},
      gameState.getRemainingFlags());

  Sprites::getInstance().copy(
      getResetButtonSprite(gameState),
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::getSettings().getResetButtonX(),
      config::getSettings().getResetButtonY());

  Sprites::getInstance().copy(
      getConfigButtonSprite(gameState),
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::getSettings().getConfigButtonX(),
      config::getSettings().getConfigButtonY());

  drawTriDigit(
      buff,
      width,
      {config::getSettings().getTimerX(),
       config::getSettings().getTimerY(),
       config::INFO_PANEL_BUTTONS_HEIGHT * 2,
       config::INFO_PANEL_BUTTONS_HEIGHT},
      gameState.getSecondsElapsed());
}

void HeaderArtist::drawRaisedResetButtonSprite(std::vector<uint32_t> &buff, const int width)
{
  draw3DCellBase(buff, width);

  FaceArtist::drawFaceBase(buff, width);
  FaceArtist::drawFaceSmile(buff, width);
  FaceArtist::drawFaceAliveEyes(buff, width);
}

void HeaderArtist::drawPressedResetButtonSprite(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);

  const double center = width * (0.5 + 0.025);
  FaceArtist::drawFaceBase(buff, width, center);
  FaceArtist::drawFaceSmile(buff, width, center);
  FaceArtist::drawFaceAliveEyes(buff, width, center);
}

void HeaderArtist::drawWinnerResetButtonSprite(std::vector<uint32_t> &buff, const int width)
{
  draw3DCellBase(buff, width);

  FaceArtist::drawFaceBase(buff, width);
  FaceArtist::drawFaceSmile(buff, width);
  FaceArtist::drawFaceShade(buff, width);
}

void HeaderArtist::drawLoserResetButtonSprite(std::vector<uint32_t> &buff, const int width)
{
  draw3DCellBase(buff, width);

  FaceArtist::drawFaceBase(buff, width);
  FaceArtist::drawFaceFrown(buff, width);
  FaceArtist::drawFaceDeadEye(buff, width);
}

void HeaderArtist::drawRaisedConfigButtonSprite(std::vector<uint32_t> &buff, const int width)
{
  draw3DCellBase(buff, width);

  HeaderArtist::drawGear(buff, width);
}

void HeaderArtist::drawPressedConfigButtonSprite(std::vector<uint32_t> &buff, const int width)
{
  draw2DCellBase(buff, width);

  const double center = width * (0.5 + 0.025);
  HeaderArtist::drawGear(buff, width, center);
}

// private

void HeaderArtist::drawTriDigit(std::vector<uint32_t> &buff, const int width, const Rect rect, const int n)
{
  BaseArtist::drawRectangle(buff, width, rect, config::Colors::BLACK);

  const int leftDigit = n / 100;
  const int middleDigit = (n % 100) / 10;
  const int rightDigit = (n % 10);
  const int digits[3]{leftDigit, middleDigit, rightDigit};

  int pad = config::getSettings().getRemainingFlagsPad();
  while ((rect.w - 4 * pad) % 3 != 0)
  {
    ++pad;
  }

  const int digitWidth = (rect.w - 4 * pad) / 3;
  const int digitHeight = rect.h - 2 * pad;

  for (int i = 0; i < 3; ++i)
  {
    const int digitX = rect.x + pad + i * (digitWidth + pad);
    const int digitY = rect.y + pad;

    drawDigit(buff, width, {digitX, digitY, digitWidth, digitHeight}, digits[i], config::Colors::RED);
  }
}

void HeaderArtist::drawGear(std::vector<uint32_t> &buff, const int width, double center)
{
  center = center < 0 ? width / 2 : center;
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double radius = size / 2 * 0.2;
  const double radiusSqrd = radius * radius;
  const double outerRadius = size / 2 * 0.5;
  const double outerRadiusSqrd = outerRadius * outerRadius;
  const double teethOuterRadius = size / 2 * 0.625;
  const double teethOuterRadiusSqrd = teethOuterRadius * teethOuterRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - center;
      const double dy = y - center;

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool innerCircle = dx * dx + dy * dy <= radiusSqrd;
      if (innerCircle)
      {
        buff[y * size + x] = config::Colors::GREY;
        continue;
      }

      const bool outerCircle = dx * dx + dy * dy <= outerRadiusSqrd;
      if (outerCircle)
      {
        buff[y * size + x] = config::Colors::BLACK;
        continue;
      }
    }
  }

  const int numTeeth = 8;
  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      const double dx = x - center;
      const double dy = y - center;
      const double distSqrd = dx * dx + dy * dy;

      if (distSqrd < outerRadiusSqrd || distSqrd > teethOuterRadiusSqrd)
      {
        continue;
      }

      double theta = std::atan2(dy, dx);
      if (theta < 0)
      {
        theta += 2 * M_PI;
      }

      const int n = theta * (numTeeth * 2) / (2 * M_PI);
      if (n % 2 != 0)
      {
        buff[y * size + x] = config::Colors::BLACK;
        continue;
      }
    }
  }
}

const std::vector<uint32_t> &HeaderArtist::getResetButtonSprite(const Minesweeper &gameState)
{
  if (gameState.getIsResetButtonPressed())
  {
    return Sprites::getInstance().get()->pressedResetButton;
  }

  if (gameState.getIsGameOver())
  {
    return gameState.getIsGameWon() ? Sprites::getInstance().get()->winnerResetButton
                                    : Sprites::getInstance().get()->loserResetButton;
  }

  return Sprites::getInstance().get()->raisedResetButton;
}

const std::vector<uint32_t> &HeaderArtist::getConfigButtonSprite(const Minesweeper &gameState)
{
  return gameState.getIsConfigButtonPressed() ? Sprites::getInstance().get()->pressedConfigButton
                                              : Sprites::getInstance().get()->raisedConfigButton;
}