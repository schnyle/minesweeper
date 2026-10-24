# minesweeper

## Components

### `app` (`src/app`): orchestrates all other components; `main.cpp` only constructs and runs `App`

pseudocode:

```
init:
    config ← Config.load()                    owns: settings, persistence
    engine ← Engine(config.board)             owns: game rules + state
    gui    ← Gui(config.display)              owns: SDL init, window, layout, drawing, pressed state
    timer  ← Timer                            owns: start/stop timestamps; elapsed computed on demand

  loop while running:
    timeout ← timer.running ? ms until next second : IDLE_WAKE (1s)
    wait for event, or timeout

    if event:
      action ← gui.handle(event)              gui never touches engine

      // std::visit + std::variant Action
      if action:                              app decides what each action means
        Quit        → running = false
        Reset       → engine.reset()          timer follows via sync_timer
        Reveal(pos) → engine.reveal(pos)
        Flag(pos)   → engine.flag(pos)
        Chord(pos)  → engine.chord(pos)

    sync_timer()                              NotStarted → reset, Playing → start once, Won/Lost → stop
    gui.render(engine, elapsed seconds)       gui stores no game state

  shutdown:
    config.save()
```

- `Timer` is generic (no engine knowledge); throws `std::logic_error` on misuse

### `config`: singleton object maintaining configuration parameters

- exposes `load()` and `save()` methods
- default template if no config exists on disk
- config file only; in-app config screen likely dropped

### `engine`: the Minesweeper game engine that maintains gamestate and handles updates

- Minesweeper game state and logic; no knowledge of time
- Build with TDD (`tests/engine`, use gtest)
- `CellPosition {row, col}`: shared position type, also used by `gui`
- `Engine::GameCell {mine, state}`, `GameCellState {Hidden, Revealed, Flagged}`: read-only via `game_cell(pos)`
- `Engine::Status {NotStarted, Playing, Won, Lost}`: first successful reveal → `Playing`; `reset()` → `NotStarted`, all cells hidden
- `reveal`/`flag`/`chord(pos)`; flag toggles Hidden ↔ Flagged; reveal on a flagged cell is a no-op
- out-of-range positions throw `std::out_of_range`

### `gui`: facade over SDL3 for displaying game state and translating input

- `Gui`: owns SDL init, window/renderer, `Layout`, and `optional<input::Press>`; exposes `handle(event)` and `render(engine, elapsed)`
- `Layout`: pixel geometry (header, reset button, board, dividers); `hit_test(point)` → `optional<HitTarget>`, `HitTarget = variant<CellPosition, ResetButton>`
- `input`: free functions; `to_action(event, layout, pressed)` → `optional<Action>`
  - `Action = variant<Reveal, Flag, Chord, Reset, Quit>` (intent structs; cell actions carry `CellPosition`)
  - Flag fires on right press; Reveal/Chord/Reset fire on release of the same button over the same `HitTarget`
  - mouse down clears any previous press

### `sprites`: generates 2d vector of colors for all the sprites used throughout the rendering

- responsible for generating reusable sprites such as flag, mine, numbers.
- deferred; skeleton draws colored rects

## Progress

- [x] Base CMake setup with `src/main.cpp`
- [x] Base GTest setup with `src/engine/` and `tests/engine`
- [ ] Skeleton architecture to test and iterate on design: app loop + window + renderer drawing plain colored rects per cell state (no sprites) + input hit testing + timer shown as text or in window title
  - [x] Base game engine
  - [ ] Layout struct (geometry)
  - [ ] Input handling
  - [ ] GUI
  - [ ] Stopwatch timer
  - [ ] Main application
- [ ] Complete `engine` functionality with TDD
- [ ] **update later**
