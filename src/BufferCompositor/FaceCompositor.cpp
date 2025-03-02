#include <FaceCompositor.hpp>
#include <cstdint>

void FaceCompositor::drawFaceBase(std::vector<uint32_t> &buff, const int width, const double center)
{
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double faceRadius = size / 2 * 0.6;
  const double faceRadiusSqrd = faceRadius * faceRadius;
  const double outlineRadiusSqrd = faceRadiusSqrd * 1.3;

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

      const bool faceCircle = dx * dx + dy * dy <= faceRadiusSqrd;

      if (faceCircle)
      {
        buff[y * size + x] = config::YELLOW;
        continue;
      }

      const bool outlineCircle = dx * dx + dy * dy <= outlineRadiusSqrd;
      if (outlineCircle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void FaceCompositor::drawFaceSmile(std::vector<uint32_t> &buff, const int width, const double center)
{
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double smileBigRadius = size / 2 * 0.4;
  const double smileBigRadiusSqrd = smileBigRadius * smileBigRadius;
  const double smileSmallRadius = size / 2 * 0.3;
  const double smileSmallRadiusSqrd = smileSmallRadius * smileSmallRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - center;
      const double dy = y - center;
      const double theta = std::atan2(dy, dx);

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool smileBigCircle = dx * dx + dy * dy <= smileBigRadiusSqrd;
      const bool smileSmallCircle = dx * dx + dy * dy <= smileSmallRadiusSqrd;
      const bool smileAngle = (theta >= 0) && (theta <= (M_PI * 2));

      if (smileBigCircle && !smileSmallCircle && smileAngle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void FaceCompositor::drawFaceFrown(std::vector<uint32_t> &buff, const int width)
{
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double faceCenter = size / 2;
  const double frownBigRadius = size / 2 * 0.4;
  const double frownBigRadiusSqrd = frownBigRadius * frownBigRadius;
  const double frownSmallRadius = size / 2 * 0.3;
  const double frownSmallRadiusSqrd = frownSmallRadius * frownSmallRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double dx = x - faceCenter;
      const double dy = y - faceCenter;
      const double theta = std::atan2(dy, dx);

      const bool inBounds = dx * dx + dy * dy <= drawRadius * drawRadius;
      if (!inBounds)
      {
        continue;
      }

      const bool smileBigCircle = dx * dx + dy * dy <= frownBigRadiusSqrd;
      const bool smileSmallCircle = dx * dx + dy * dy <= frownSmallRadiusSqrd;
      const bool smileAngle = (theta >= 0) && (theta <= (M_PI * 2));

      if (smileBigCircle && !smileSmallCircle && smileAngle)
      {
        buff[(size - y + (size / 5)) * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void FaceCompositor::drawFaceAliveEyes(std::vector<uint32_t> &buff, const int width, const double center)
{
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double leftEyeX = center * 13 / 16;
  const double leftEyeY = center * 3 / 4;
  const double rightEyeX = center * 19 / 16;
  const double rightEyeY = center * 3 / 4;
  const double eyeRadius = size / 2 * 0.1;
  const double eyeRadiusSqrd = eyeRadius * eyeRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double leftDx = x - leftEyeX;
      const double leftDy = y - leftEyeY;

      const double rightDx = x - rightEyeX;
      const double rightDy = y - rightEyeY;

      const bool leftInBounds = leftDx * leftDx + leftDy * leftDy <= drawRadius * drawRadius;
      const bool rightInBounds = rightDx * rightDx + rightDy * rightDy <= drawRadius * drawRadius;
      if (!leftInBounds && !rightInBounds)
      {
        continue;
      }

      const bool leftCircle = leftDx * leftDx + leftDy * leftDy <= eyeRadiusSqrd;
      const bool rightCircle = rightDx * rightDx + rightDy * rightDy <= eyeRadiusSqrd;

      if (leftCircle || rightCircle)
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
};

void FaceCompositor::drawFaceDeadEye(std::vector<uint32_t> &buff, const int width)
{
  const int size = width;
  const double leftEyeX = size * 25 / 64;
  const double leftEyeY = size * 3 / 8;
  const double rightEyeX = size * 39 / 64;
  const double rightEyeY = size * 3 / 8;
  const double eyeRadius = size / 8;
  const int xLineThickness = 2;

  drawX(buff, width, config::BLACK, leftEyeX, leftEyeY, eyeRadius, xLineThickness);
  drawX(buff, width, config::BLACK, rightEyeX, rightEyeY, eyeRadius, xLineThickness);
}

void FaceCompositor::drawFaceShade(std::vector<uint32_t> &buff, const int width)
{
  const int size = width;
  const double drawRadius = size / 2 * 0.9;
  const double leftEyeX = size * 12 / 32;
  const double leftEyeY = size * 3 / 8;
  const double rightEyeX = size * 20 / 32;
  const double rightEyeY = size * 3 / 8;
  const double eyeRadius = size / 2 * 0.2;
  const double eyeRadiusSqrd = eyeRadius * eyeRadius;

  for (int y = 0; y < size; y++)
  {
    for (int x = 0; x < size; x++)
    {
      const double leftDx = x - leftEyeX;
      const double leftDy = y - leftEyeY;
      const double leftTheta = std::atan2(leftDy, leftDx);

      const double rightDx = x - rightEyeX;
      const double rightDy = y - rightEyeY;
      const double rightTheta = std::atan2(rightDy, rightDx);

      const bool leftInBounds = leftDx * leftDx + leftDy * leftDy <= drawRadius * drawRadius;
      const bool rightInBounds = rightDx * rightDx + rightDy * rightDy <= drawRadius * drawRadius;
      if (!leftInBounds && !rightInBounds)
      {
        continue;
      }

      const bool leftCircle = leftDx * leftDx + leftDy * leftDy <= eyeRadiusSqrd;
      const bool rightCircle = rightDx * rightDx + rightDy * rightDy <= eyeRadiusSqrd;

      const bool leftSemiCircle = (leftTheta >= 0) && (leftTheta <= (M_PI * 2));
      const bool rightSemiCircle = (leftTheta >= 0) && (rightTheta <= (M_PI * 2));

      if ((leftCircle && leftSemiCircle) || (rightCircle && rightSemiCircle))
      {
        buff[y * size + x] = config::BLACK;
        continue;
      }
    }
  }
}

void FaceCompositor::drawX(
    std::vector<uint32_t> &buff,
    const int width,
    const uint32_t color,
    const double xCenter,
    const double yCenter,
    const double xSize,
    const double xThickness)
{
  const int size = width;
  const double xRadius = xThickness / std::sqrt(2);

  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      const int dx = x - xCenter;
      const int dy = y - yCenter;

      if (std::abs(dx) > xSize / 2 || std::abs(dy) > xSize / 2)
      {
        continue;
      }

      const bool positiveDiag = dy <= dx + xRadius && dy >= dx - xRadius;
      const bool negativeDiag = dy <= -dx + xRadius && dy >= -dx - xRadius;

      if (positiveDiag || negativeDiag)
      {
        buff[y * size + x] = color;
      }
    }
  }
}