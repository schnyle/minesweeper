#include "engine/engine.hpp"

#include <format>
#include <gtest/gtest.h>
#include <ranges>
#include <stdexcept>

// TODO: when adding a Fixture, consider a helper that checks the entire board, e.g. some
//     void expect_board(const Engine &engine, std::invocable<CellPosition> auto expected_state)
//     {
//         for (auto [r, c] : std::views::cartesian_product(std::views::iota(0, engine.row_count()),
//                                                           std::views::iota(0, engine.col_count())))
//         {
//             const CellPosition pos{.row = r, .col = c};
//             EXPECT_EQ(engine.game_cell(pos).state, expected_state(pos)) << std::format("cell ({} {})", r, c);
//         }
//     }

namespace
{
constexpr int ROWS = 10;
constexpr int COLS = 10;
}; // namespace

// Input Validation

TEST(EngineTest, ThrowsOutOfRangeOnOutOfBoundsCell)
{
  Engine engine(ROWS, COLS);

  EXPECT_NO_THROW((void)engine.game_cell({0, 0}));
  EXPECT_THROW((void)engine.game_cell({-1, 0}), std::out_of_range);
  EXPECT_THROW((void)engine.game_cell({0, -1}), std::out_of_range);

  EXPECT_NO_THROW((void)engine.game_cell({ROWS - 1, COLS - 1}));
  EXPECT_THROW((void)engine.game_cell({ROWS, 0}), std::out_of_range);
  EXPECT_THROW((void)engine.game_cell({0, COLS}), std::out_of_range);
}

// Initialization

TEST(EngineTest, InitCorrectBoardDimensions)
{
  Engine engine(ROWS, COLS);

  EXPECT_EQ(ROWS, engine.row_count());
  EXPECT_EQ(COLS, engine.col_count());
}

TEST(EngineTest, InitAllCellsHidden)
{
  Engine engine(ROWS, COLS);

  for (auto [r, c] : std::views::cartesian_product(std::views::iota(0, ROWS), std::views::iota(0, COLS)))
  {
    EXPECT_EQ(engine.game_cell({.row = r, .col = c}).state, Engine::GameCellState::Hidden);
  }
}

TEST(EngineTest, InitNotStarted)
{
  Engine engine(ROWS, COLS);

  EXPECT_EQ(engine.status(), Engine::Status::NotStarted);
}

// Chord

TEST(EngineTest, ChordBasic)
{
  Engine engine(ROWS, COLS);

  engine.chord({.row = 2, .col = 2});

  for (auto [r, c] : std::views::cartesian_product(std::views::iota(1, 4), std::views::iota(1, 4)))
  {
    const Engine::GameCellState state = engine.game_cell({.row = r, .col = c}).state;
    EXPECT_EQ(state, Engine::GameCellState::Revealed) << std::format("cell ({}, {})", r, c);
  }
}

// Flag

TEST(EngineTest, FlagSetsHiddenToFlagged)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  ASSERT_EQ(engine.game_cell(pos).state, Engine::GameCellState::Hidden);
  engine.flag(pos);
  EXPECT_EQ(engine.game_cell(pos).state, Engine::GameCellState::Flagged);
}

TEST(EngineTest, FlagSetsFlaggedToHidden)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  engine.flag(pos);
  ASSERT_EQ(engine.game_cell(pos).state, Engine::GameCellState::Flagged);
  engine.flag(pos);
  EXPECT_EQ(engine.game_cell(pos).state, Engine::GameCellState::Hidden);
}

// Reveal

TEST(EngineTest, RevealBasic)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  engine.reveal(pos);

  const Engine::GameCell cell = engine.game_cell(pos);
  EXPECT_EQ(cell.state, Engine::GameCellState::Revealed);
}

TEST(EngineTest, FirstRevealStartsGame)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  engine.reveal(pos);
  EXPECT_EQ(engine.status(), Engine::Status::Playing);
}

TEST(EngineTest, FirstRevealOnFlaggedDoesNotStartGame)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  engine.reveal(pos);
  EXPECT_EQ(engine.status(), Engine::Status::Playing);
}

TEST(EngineTest, RevealOnFlaggedCellIsNoop)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  engine.flag(pos);
  ASSERT_EQ(engine.game_cell(pos).state, Engine::GameCellState::Flagged);

  engine.reveal(pos);
  EXPECT_EQ(engine.status(), Engine::Status::NotStarted);
}

// Reset

TEST(EngineTest, ResetSetsAllCellsToHidden)
{
  Engine engine(ROWS, COLS);
  constexpr CellPosition pos{.row = 0, .col = 0};

  engine.reveal(pos);
  engine.flag(pos);
  engine.reset();

  for (auto [r, c] : std::views::cartesian_product(std::views::iota(0, ROWS), std::views::iota(0, COLS)))
  {
    EXPECT_EQ(engine.game_cell({.row = r, .col = c}).state, Engine::GameCellState::Hidden);
  }
}

TEST(EngineTest, ResetEndsGame)
{
  Engine engine(ROWS, COLS);
  engine.reveal({.row = 0, .col = 0}); // start game
  engine.reset();

  EXPECT_EQ(engine.status(), Engine::Status::NotStarted);
}
