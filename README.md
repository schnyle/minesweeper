# minesweeper

A Minesweeper implementation in C++17 built from scratch on SDL2, with a component-based architecture, RAII-managed SDL resources, and a build that produces both a statically linked Linux binary and a cross-compiled Windows executable.

## Architecture

- **Component-based design** — game logic, windowing, and drawing are separated into distinct components: a `Window` layer (game and settings windows), an `Artist` layer (`MinefieldArtist`, `HeaderArtist`, `FaceArtist` over a shared `BaseArtist`), and the core `Minesweeper` game state and loop.
- **Singleton sprite manager** — sprites are generated and served through a single `Sprites` instance (Meyers singleton).
- **Recursive flood-fill reveal** — revealing an empty cell recursively opens its empty neighbors, tracking visited cells with a `std::set`.

## Modern C++ / RAII

- SDL and SDL_ttf resources (`SDL_Window`, `SDL_Renderer`, `SDL_Texture`, `TTF_Font`) are held in `std::unique_ptr`s with custom deleters, so cleanup is automatic and exception-safe.
- `std::filesystem` handles the config file path (`~/.config/minesweeper.conf`).
- The UI font is embedded into the binary as a byte array and loaded from memory (`SDL_RWFromMem`), so the executable is self-contained with no external asset files.

## Build

Requires a C++17 compiler, CMake 3.18+, and SDL2 / SDL2_ttf development libraries.

**Dynamic build** (fast; requires SDL2 installed):

```bash
mkdir build && cd build
cmake ..
make
```

**Static release build** (standalone binary):

```bash
mkdir build-release && cd build-release
cmake -DSTATIC_BUILD=ON ..
make
```

**Windows cross-compilation from Linux** (via Docker + MinGW toolchain):

```bash
docker build -t minesweeper-windows build-tools/windows
docker run -v $(pwd):/src minesweeper-windows
```
