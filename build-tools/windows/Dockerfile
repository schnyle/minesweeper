# from project root:
# sudo docker build -t minesweeper-windows-build build-tools/windows
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
    zip \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Copy toolchain and build files - note we're copying from the same directory as Dockerfile
COPY build.sh /build.sh
COPY windows-toolchain.cmake /windows-toolchain.cmake
RUN chmod +x /build.sh

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

CMD ["/build.sh"]