#!/bin/bash
set -x
mkdir -p build && cd build

PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/lib/pkgconfig \
cmake -DCMAKE_TOOLCHAIN_FILE=/windows-toolchain.cmake \
      -DSDL2_DIR=/usr/x86_64-w64-mingw32/lib/cmake/SDL2 \
      -DCMAKE_VERBOSE_MAKEFILE=ON ..

make VERBOSE=1 -j$(nproc)

mkdir -p dist/minesweeper
cp minesweeper.exe dist/minesweeper/
cp /usr/x86_64-w64-mingw32/bin/SDL2.dll dist/minesweeper/
cp /usr/x86_64-w64-mingw32/bin/SDL2_ttf.dll dist/minesweeper/
cp /usr/lib/gcc/x86_64-w64-mingw32/*-posix/libstdc++-6.dll dist/minesweeper/
cp /usr/lib/gcc/x86_64-w64-mingw32/*-posix/libgcc_s_seh-1.dll dist/minesweeper/
cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll dist/minesweeper/

cd dist
zip -r ../minesweeper.zip minesweeper