# notes

| Situation                                                 | Command                                                              |
| --------------------------------------------------------- | -------------------------------------------------------------------- |
| First time                                                | `cmake -S . -B build`                                                |
| Normal development (edited `.cpp`/`.hpp` or a CMakeLists) | `cmake --build build`                                                |
| Run tests                                                 | `ctest --test-dir build --output-on-failure`                         |
| Change a cache setting                                    | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release` (or `ccmake build`) |
| Something seems stale or broken                           | `cmake --build build --clean-first`                                  |
| Start from scratch                                        | `rm -rf build` then configure, or `cmake --fresh -S . -B build`      |

| Type      | Compiles sources? | Produces                      | Linking against it gives you                             |
| --------- | ----------------- | ----------------------------- | -------------------------------------------------------- |
| STATIC    | Yes               | `libengine.a` (archive of .o) | Its usage requirements + the archive linked in           |
| SHARED    | Yes               | `libengine.so`                | Its usage requirements + a runtime dependency on the .so |
| OBJECT    | Yes               | Loose .o files, no archive    | Its usage requirements + the .o files added directly     |
| INTERFACE | No                | Nothing                       | Only its usage requirements                              |
