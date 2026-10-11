#include "engine.hpp"

Engine::Engine(int rows, int cols)
    : row_count_(rows), col_count_(cols), cells_(static_cast<size_t>(rows) * static_cast<size_t>(cols))
{
}

const Engine::Cell &Engine::cell(int row, int col) const { return cell_impl(row, col); }

void Engine::reveal(int row, int col) { cell_impl(row, col).state = CellState::Revealed; };

void Engine::flag(int row, int col) { cell_impl(row, col).state = CellState::Flagged; }

void Engine::chord(int row, int col) { cell_impl(row, col).state = CellState::Revealed; };
