# minesweeper

## Components

### `app`: root of process which orchestrates all other components

pseudocode:

```
init:
    init SDL                                  (before renderer creates a window)
    config   ← Config.load()                  owns: settings, persistence
    engine   ← Engine(config.board)           owns: game rules + state
    renderer ← Renderer(config.display)       owns: window, layout, drawing
    input    ← InputHandler                   owns: UI interaction state (pressed button/cell)
    timer    ← Timer                          owns: start/stop timestamps; elapsed computed on demand
    screen   ← Game                           app owns current screen (Game | Config)

  loop while running:
    timeout ← timer.running ? ms until next second : forever
    wait for event, or timeout

    if event:
      action ← input.handle(event, renderer.layout, screen)     input never touches engine/renderer

      // std::visit + std::variant Action
      if action:                                                 app decides what each action means
        Quit        → running = false
        Reset       → engine.reset(); timer.reset()
        OpenConfig  → screen = Config
        CloseConfig → screen = Game; apply config changes to engine/renderer
        Reveal(r,c) → engine.reveal(r, c)
        Flag(r,c)   → engine.flag(r, c)
        Chord(r,c)  → engine.chord(r, c)

    timer.sync(engine.status)                 start on first reveal, stop on Won/Lost
    renderer.render(engine.state, input.ui_state, screen, timer.elapsed)   all inputs passed, renderer stores none

  shutdown:
    config.save()
```

### `config`: singleton object maintaining configuration parameters

- exposes `load()` and `save()` methods
- default template if no config exists on disk

### `engine`: the Minesweeper game engine that maintains gamestate and handles updates

- Minesweeper game state and logic
- Build with TDD (`tests/engine`, use gtest)

### `input`: handles mouse events

- exposes a helper function to take an `SDL_Event&` and `Layout&` to produce an `Action` enum result
- holds a small amount of state to handle consecutive `SDL_MOUSEBUTTONUP`/`SDL_MOUSEBUTTONDOWN` events

### `renderer`: manages SDL2 for displaying the Minesweeper game state and surrounding GUI

### `sprites`: generates 2d vector of colors for all the sprites used throughout the rendering

- responsible for generating reusable sprites such as flag, mine, numbers.

## Progress

- [x] Base CMake setup with `src/main.cpp`
- [x] Base GTest setup with `src/engine/` and `tests/engine`
- [ ] Basic public `engine` functionality with TDD
- [ ] Skeleton architecture to test design: app loop + window + renderer drawing plain colored rects per cell state (no sprites) + input hit testing + timer shown as text or in window title
- [ ] Iterate on design
- [ ] Complete `engine` functionality with TDD
- [ ] **update later**
