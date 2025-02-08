# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set non-interactive frontend to avoid issues in package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages for OpenGL, GLFW, and other dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    ninja-build \
    build-essential \
    libglfw3-dev \
    libglew-dev \
    libassimp-dev \
    libsoil-dev \
    libglm-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    zlib1g-dev \
    libglu1-mesa-dev \
    libgl1-mesa-glx \
    libglvnd-dev \
    libegl1-mesa-dev \
    libgles2-mesa-dev \
    libx11-dev \
    libxext-dev \
    x11-xserver-utils \
    xorg \
    mesa-utils \
    sudo \
    pkg-config \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /app

# Clone tinyfiledialogs manually (since it's not in apt)
RUN git clone https://github.com/native-toolkit/tinyfiledialogs.git /app/external/tinyfiledialogs

# Copy the project files BEFORE running CMake (to avoid unnecessary rebuilds)
COPY . .

# Create necessary directories
RUN mkdir -p /app/build /app/bin /app/lib /app/external /app/bin/shaders /app/bin/resources

# Ensure shaders and resources are correctly copied
RUN cp -r /app/shaders/* /app/bin/shaders/
RUN cp -r /app/resources/* /app/bin/resources/

# Ensure correct permissions for shaders
RUN chmod -R 755 /app/bin/shaders /app/bin/resources

# Configure and build the project
WORKDIR /app/build
RUN cmake .. -G Ninja && ninja

# Ensure ClueEngine starts in the correct directory
WORKDIR /app/bin

# Use the same DISPLAY variable as the host
ENV DISPLAY=$DISPLAY  

# Ensure permissions for OpenGL & sound devices
RUN usermod -aG video,audio root && \
    chmod -R 777 /dev/dri /dev/snd || true

# Set correct permissions at runtime and execute ClueEngine
CMD bash -c "\
    if [ -d /tmp/.X11-unix ]; then chmod -R 777 /tmp/.X11-unix; fi && \
    chmod +x /app/bin/ClueEngine && \
    exec /app/bin/ClueEngine"
