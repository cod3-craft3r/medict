FROM ubuntu:22.04
LABEL maintainer="AstroCat"
LABEL description="Dockerfile for setting up a development environment for *medict*"

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    bash \
    build-essential \
    cmake \
    curl \
    wget \
    git \
    vim \
    ca-certificates

# Install SDL2 libraries
RUN apt-get install -y --no-install-recommends \
    libsdl2-dev \
    libsdl2-2.0-0 \
    libsdl2-image-2.0-0 \
    libsdl2-image-dev

# Install X11 libraries
RUN apt-get install -y --no-install-recommends \
    x11-apps \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev

# Install Wayland libraries
RUN apt-get install -y --no-install-recommends \
    libwayland-dev \
    libwayland-client0 \
    libwayland-cursor0 \
    libwayland-egl1 \
    libxkbcommon-dev \
    libxkbcommon-x11-dev

# RUN apt-get install -y software-properties-common \
#     && add-apt-repository ppa:ubuntu-toolchain-r/test -y \
#     && apt-get update \
#     && apt-get install -y gcc-13 g++-13 \
#     && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 60 \
#     && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 60

RUN apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-13 g++-13 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 13 --slave /usr/bin/g++ g++ /usr/bin/g++-13 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /work

COPY . .

CMD ["/bin/bash"]
