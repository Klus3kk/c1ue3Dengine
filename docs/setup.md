# Setup Guide for ClueEngine

## Introduction

This guide will help you set up **ClueEngine** on your system using **CMake** for building the project and **Docker** for creating a containerized development environment. Most of the necessary dependencies (such as GLAD, SOIL2, GLEW, etc.) are included in the repository, but you will need to install **GLFW** manually on Linux/macOS.

## Prerequisites

Before setting up **ClueEngine**, ensure the following tools are installed on your system:

- **CMake** (for building the project)
- **C++ Compiler** (e.g., GCC, Clang, MSVC)
- **GLFW** (Required on Linux/macOS)
- **Docker** (optional for containerized setup)
- **Visual Studio** (Windows only, for `.sln` support)

If you prefer using Docker, **ClueEngine** provides a Dockerfile that automatically installs dependencies and sets up the environment.

## Installing Dependencies

### Windows
GLFW is included in the repository, so no extra steps are needed.

### Linux
Install GLFW3 using your package manager:
```bash
# Ubuntu/Debian
sudo apt update && sudo apt install libglfw3-dev

# Arch Linux
sudo pacman -S glfw-x11   # Use glfw-wayland if on Wayland

# Fedora
sudo dnf install glfw-devel

# openSUSE
sudo zypper install glfw-devel
```

### macOS
Install GLFW3 using Homebrew:
```bash
brew install glfw
```

## CMake Setup (Preferred Method)

### Step 1: Clone the Repository

```bash
git clone https://github.com/Klus3kk/ClueEngine.git
cd ClueEngine
```

### Step 2: Build the Project

Use **CMake** to configure and build the project:

```bash
cmake -B build
cmake --build build
```

This will compile the source code and generate the **ClueEngine** executable in the `bin` directory.

### Step 3: Run the Engine

Once the project is built, you can run the engine executable:

- On Linux/macOS:

```bash
./bin/ClueEngine
```

- On Windows, you can use the `.exe` file located in the `./bin` directory:

```bash
./bin/ClueEngine.exe
```

## Visual Studio Setup (Windows Only)

For users on Windows, **ClueEngine** also provides a **Visual Studio Solution** file (`ClueEngine.sln`) that allows you to open and build the project directly in Visual Studio.

### Step 1: Open the Solution

- Launch **Visual Studio**.
- Open the `ClueEngine.sln` file located in the root directory of the project.

### Step 2: Build the Project

- In **Visual Studio**, right-click the **ClueEngine** project in the **Solution Explorer** and select **Build**.
- This will compile the project and generate the executable in the `bin` directory.

### Step 3: Run the Engine

- Once the build is successful, run the project directly from Visual Studio or navigate to the `./bin` folder and run the executable:

```bash
./bin/ClueEngine.exe
```

## Docker Setup (Alternative Method)

If you prefer an isolated environment, you can set up **ClueEngine** using Docker. This method ensures that the required dependencies are bundled into the container and isolates the environment.

### Step 1: Install Docker

Follow the instructions on the [Docker website](https://docs.docker.com/get-docker/) to install Docker on your system.

### Step 2: Build the Docker Image

Navigate to the project directory and build the Docker image:

```bash
docker build -t clueengine .
```

### Step 3: Run the Docker Container

Run the Docker container with the following command:

```bash
docker run --rm -it --gpus all -v $(pwd):/app clueengine
```

This command will start the container and run **ClueEngine** within the isolated environment.

## Next Steps

After successfully building and running **ClueEngine**, you can start creating your 3D scenes! Refer to the [User Guide](docs/userguide.md) for instructions on how to create and manage objects, materials, lights, and more.

