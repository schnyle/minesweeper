# sudo docker build -t minesweeper-windows-build .
# sudo docker run -v $(pwd):/src minesweeper-windows-build

FROM debian:bullseye

# Install basic build tools and wget
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    gcc \
    g++ \
    mingw-w64 \
    wget \
    git \
    xxd \
    pkg-config \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Create toolchain file first so we can use it for SDL2 builds
RUN echo "set(CMAKE_SYSTEM_NAME Windows)" > /windows-toolchain.cmake \
    && echo "set(CMAKE_SYSTEM_PROCESSOR x86_64)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)" >> /windows-toolchain.cmake \
    && echo "set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)" >> /windows-toolchain.cmake

# Set up directories for dependencies
WORKDIR /opt/build
ENV SDL2_VERSION=2.28.5
ENV SDL2_TTF_VERSION=2.20.2

# Download and build SDL2
RUN wget https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-mingw.tar.gz \
    && tar xzf SDL2-devel-${SDL2_VERSION}-mingw.tar.gz \
    && cp -r SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/lib/* /usr/x86_64-w64-mingw32/lib/ \
    && cp -r SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/include/* /usr/x86_64-w64-mingw32/include/ \
    && cp SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/bin/SDL2.dll /usr/x86_64-w64-mingw32/bin/

# Download and build SDL2_ttf
RUN wget https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/SDL2_ttf-devel-${SDL2_TTF_VERSION}-mingw.tar.gz \
    && tar xzf SDL2_ttf-devel-${SDL2_TTF_VERSION}-mingw.tar.gz \
    && cp -r SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/lib/* /usr/x86_64-w64-mingw32/lib/ \
    && cp -r SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/include/* /usr/x86_64-w64-mingw32/include/ \
    && cp SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/bin/*.dll /usr/x86_64-w64-mingw32/bin/

# Create working directory
WORKDIR /src

# Build script with DLL copying
RUN echo '#!/bin/bash' > /build.sh \
    && echo 'set -x' >> /build.sh \
    && echo 'mkdir -p build && cd build' >> /build.sh \
    && echo 'PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/lib/pkgconfig \
             cmake -DCMAKE_TOOLCHAIN_FILE=/windows-toolchain.cmake \
             -DSDL2_DIR=/usr/x86_64-w64-mingw32/lib/cmake/SDL2 \
             -DCMAKE_VERBOSE_MAKEFILE=ON ..' >> /build.sh \
    && echo 'make VERBOSE=1 -j$(nproc)' >> /build.sh \
    && echo 'mkdir -p dist' >> /build.sh \
    && echo 'cp minesweeper.exe dist/' >> /build.sh \
    && echo 'cp /usr/x86_64-w64-mingw32/bin/SDL2.dll dist/' >> /build.sh \
    && echo 'cp /usr/x86_64-w64-mingw32/bin/SDL2_ttf.dll dist/' >> /build.sh \
    && echo 'cp /usr/lib/gcc/x86_64-w64-mingw32/*-posix/libstdc++-6.dll dist/' >> /build.sh \
    && echo 'cp /usr/lib/gcc/x86_64-w64-mingw32/*-posix/libgcc_s_seh-1.dll dist/' >> /build.sh \
    && echo 'cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll dist/' >> /build.sh \
    && chmod +x /build.sh

CMD ["/build.sh"]