FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    libgl1-mesa-dev \
    libglfw3-dev \
    libglew-dev \
    libassimp-dev \
    libsoil-dev \
    libtinyfiledialogs-dev \
    xorg \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN mkdir -p build
RUN cd build && cmake .. && make -j$(nproc)
CMD ["./bin/ClueEngine"]
