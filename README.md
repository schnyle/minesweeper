# Minesweeper

## Technical Highlights

### Modern C++ Design Patterns

#### RAII (Resource Acquisition is Initialization)

- Customer smart pointer wrappers for SDL resources with automatic cleanup
- Exception-safe resource management throughout codebase
- Zero memory leaks through proper RAII implementation

#### Modern C++17 Features

- Structured bindings for elegant iteration patterns
- `std::filesystem` for cross-platform path handling
- `std::unique_ptr` with custom deleters for SDL resource management
- Range-based for loops with structured bindings

### Software Architecture

#### Component-Based Architecture

- Separation of concerns with distinct Artist, Window, and Game Logic components
- Abstract base classes with virtual interfaces for extensibility
- Modular design allowing easy feature additions

#### Observer Pattern Implementation

- Event-driven architecture for user input handling
- Decoupled game state from rendering logic
- Clean separation between model, view, and controller

#### Singleton Pattern with Thread Safety

- Thread-safe singleton implementation for global settings and sprite management
- Lazy initialization with proper resource cleanup

### Performance Optimizations

#### Memory Management

- Pre-allocated frame buffers to avoid runtime allocations
- Efficient sprite caching system reducing redundant computations
- Stack-based algorithms for flood-fill operations

#### Algorithmic Efficiency

- Optimized flood-fill algorithm using iterative approach with 'visited' set
- Efficient adjacency calculations using lookup tables
- Minimal memory footprint with bit-packed cell states

### Cross-Platform Development

#### Build System Engineering

- CMake-based build system with cross-compilation support
- Docker containerization for Windows cross-compilation from Linux
- Automated dependency management for linking

#### Platform Abstraction

- SDL-2 based rending for cross-platform compatibility
- Platform specific optimizations while maintaining code portability
- Embedded font resources using `xxd` for self-contained executables

### Graphics Programming

#### Custom Rendering Engine

- Software-based pixel manipulation for precise control
- Custom spite generation and caching system
- Efficient texture streaming and memory management

#### Mathematical Precision

- Floating-point arithmetic for smooth animations and positioning
- Geometric algorithms for UI element positioning
- Color space manipulation and blending

## Build Requirements

- C++17 compatible compiler
- CMake 3.18+
- SDL2 and SDL2_ttf development libraries

## Cross-Platform Compilation

### Linux/macOD

```bash
mkdir build && cd build
cmake ..
make
```

### Windows (cross-compilation)

```bash
docker build -t minesweeper-windows build-tools/windows
docker run -v $(pwd):/src minesweeper-windows
```
