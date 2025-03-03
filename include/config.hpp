#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

namespace config
{
struct Colors
{
  static constexpr uint32_t BLACK = 0x000000ff;
  static constexpr uint32_t LIGHT_BLUE = 0x89cff0ff;
  static constexpr uint32_t BLUE = 0x0000ffff;
  static constexpr uint32_t DARK_BLUE = 0x01007aff;
  static constexpr uint32_t DARK_GREY = 0x7a7a7bff;
  static constexpr uint32_t DARK_RED = 0x840101ff;
  static constexpr uint32_t GREEN = 0x017a00ff;
  static constexpr uint32_t GREY = 0xbdbdbdff;
  static constexpr uint32_t LIGHT_GREY = 0xfefffeff;
  static constexpr uint32_t PURPLE = 0x850085ff;
  static constexpr uint32_t RED = 0xff0000ff;
  static constexpr uint32_t TURQUOISE = 0x018384ff;
  static constexpr uint32_t YELLOW = 0xffff00ff;
  static constexpr uint32_t WHITE = 0xffffffff;
};

// App settings
constexpr const char *APP_NAME = "Minesweeper";
constexpr int FRAME_WIDTH = 20;
constexpr int INFO_PANEL_HEIGHT = 70;
constexpr int INFO_PANEL_BUTTONS_HEIGHT = 0.75 * INFO_PANEL_HEIGHT;
constexpr int CELL_BORDER_WIDTH_2D = 2; // even int
constexpr int DEFAULT_CELL_PIXEL_SIZE = 50;
constexpr double DEFAULT_GAME_WINDOW_TO_DISPLAY_RATIO = 0.7;

inline std::filesystem::path getConfigPath()
{
  const char *home = std::getenv("HOME");
  if (!home)
  {
    return {};
  }
  return std::filesystem::path(home) / ".config" / "minesweeper.conf";
}

class Settings
{
public:
  void initialize(const int dw, const int dh)
  {
    displayWidth = dw;
    displayHeight = dh;

    const auto configPath = getConfigPath();

    if (!std::filesystem::exists(configPath))
    {
      cellPixelSize = DEFAULT_CELL_PIXEL_SIZE;
      gameWindowWidth = displayWidth * DEFAULT_GAME_WINDOW_TO_DISPLAY_RATIO;
      gameWindowHeight = displayHeight * DEFAULT_GAME_WINDOW_TO_DISPLAY_RATIO;

      writeToFile();
    }
    else
    {
      readFromFile();
    }

    updateDerivedValues();
  }

  bool writeToFile(int newGameWindowWidth = -1, int newGameWindowHeight = -1, int newCellPixelSize = -1) const
  {
    newGameWindowWidth = newGameWindowWidth < 0 ? gameWindowWidth : newGameWindowWidth;
    newGameWindowHeight = newGameWindowHeight < 0 ? gameWindowHeight : newGameWindowHeight;
    newCellPixelSize = newCellPixelSize < 0 ? cellPixelSize : newCellPixelSize;

    auto configPath = getConfigPath();
    if (configPath.empty())
    {
      return false;
    }

    try
    {
      std::ofstream ofs(configPath);
      ofs << "GAME_WINDOW_PIXEL_WIDTH=" << newGameWindowWidth << "\n";
      ofs << "GAME_WINDOW_PIXEL_HEIGHT=" << newGameWindowHeight << "\n";
      ofs << "CELL_PIXEL_SIZE=" << newCellPixelSize << "\n";

      const bool success = ofs.good();
      ofs.close();
      return success;
    }
    catch (...)
    {
      return false;
    }
  };

  int getDisplayWidth() const { return displayWidth; }
  int getDisplayHeight() const { return displayHeight; }
  int getGameWindowWidth() const { return gameWindowWidth; }
  int getGameWindowHeight() const { return gameWindowHeight; }
  int getCellPixelSize() const { return cellPixelSize; }
  int getConfigWindowWidth() const { return configWindowWidth; }
  int getConfigWindowHeight() const { return configWindowHeight; }
  int getResetButtonX() const { return resetButtonX; }
  int getResetButtonY() const { return resetButtonY; }
  int getRemainingFlagsX() const { return remainingFlagsX; }
  int getRemainingFlagsY() const { return remainingFlagsY; }
  int getRemainingFlagsPad() const { return remainingFlagsPad; }
  int getTimerX() const { return timerX; }
  int getTimerY() const { return timerY; }
  int getConfigButtonX() const { return configButtonX; }
  int getConfigButtonY() const { return configButtonY; }
  int getGameAreaWidth() const { return gameAreaWidth; }
  int getGameAreaHeight() const { return gameAreaHeight; }
  int getGridWidth() const { return gridWidth; }
  int getGridHeight() const { return gridHeight; }
  int getGridAreaPadX() const { return gridAreaPadX; }
  int getGridAreaPadY() const { return gridAreaPadY; }
  int getCellBorderWidth3D() const { return cellBorderWidth3D; }

private:
  void readFromFile()
  {
    auto configPath = getConfigPath();
    if (configPath.empty() || !std::filesystem::exists(configPath))
    {
      return;
    }

    std::map<std::string, int *> settingsMap = {
        {"GAME_WINDOW_PIXEL_WIDTH", &gameWindowWidth},
        {"GAME_WINDOW_PIXEL_HEIGHT", &gameWindowHeight},
        {"CELL_PIXEL_SIZE", &cellPixelSize}};

    std::ifstream ifs(configPath);
    std::string line;

    while (std::getline(ifs, line))
    {
      auto delimPos = line.find('=');
      if (delimPos == std::string::npos)
      {
        continue;
      }

      std::string key = line.substr(0, delimPos);
      std::string value = line.substr(delimPos + 1);

      auto it = settingsMap.find(key);
      if (it == settingsMap.cend())
      {
        continue;
      }
      try
      {
        *(it->second) = std::stoi(value);
      }
      catch (...)
      {
        // Ignore conversion errors
      }
    }
  }

  void updateDerivedValues()
  {
    configWindowWidth = gameWindowHeight / 2;
    configWindowHeight = gameWindowHeight / 2;

    resetButtonX = gameWindowWidth / 2 - INFO_PANEL_BUTTONS_HEIGHT / 2;
    resetButtonY = FRAME_WIDTH + INFO_PANEL_HEIGHT / 2 - INFO_PANEL_BUTTONS_HEIGHT / 2;

    remainingFlagsX = 2 * FRAME_WIDTH;
    remainingFlagsY = FRAME_WIDTH + INFO_PANEL_HEIGHT / 2 - INFO_PANEL_BUTTONS_HEIGHT / 2;
    remainingFlagsPad = 0.05 * INFO_PANEL_HEIGHT;

    timerX = gameWindowWidth - remainingFlagsX - 2 * INFO_PANEL_BUTTONS_HEIGHT;
    timerY = remainingFlagsY;

    configButtonX = timerX - INFO_PANEL_BUTTONS_HEIGHT * 2;
    configButtonY = resetButtonY;

    gameAreaWidth = gameWindowWidth - 2 * FRAME_WIDTH;
    gameAreaHeight = gameWindowHeight - INFO_PANEL_HEIGHT - 3 * FRAME_WIDTH;

    gridWidth = gameAreaWidth / cellPixelSize;
    gridHeight = gameAreaHeight / cellPixelSize;

    gridAreaPadX = (gameAreaWidth % cellPixelSize) / 2;
    gridAreaPadY = (gameAreaHeight % cellPixelSize) / 2;

    cellBorderWidth3D = cellPixelSize / 10;
  }

  // primary
  int displayWidth = 0;
  int displayHeight = 0;
  int gameWindowWidth = 0;
  int gameWindowHeight = 0;
  int cellPixelSize = DEFAULT_CELL_PIXEL_SIZE;

  // derived
  int configWindowWidth = 0;
  int configWindowHeight = 0;
  int resetButtonX = 0;
  int resetButtonY = 0;
  int remainingFlagsX = 0;
  int remainingFlagsY = 0;
  int remainingFlagsPad = 0;
  int timerX = 0;
  int timerY = 0;
  int configButtonX = 0;
  int configButtonY = 0;
  int gameAreaWidth = 0;
  int gameAreaHeight = 0;
  int gridWidth = 0;
  int gridHeight = 0;
  int gridAreaPadX = 0;
  int gridAreaPadY = 0;
  int cellBorderWidth3D = 0;
};

inline Settings &getSettings()
{
  static Settings instance;
  return instance;
}

} // namespace config