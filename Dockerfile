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

WORKDIR /work

COPY . .

CMD ["/bin/bash"]
