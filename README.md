# C1ue Engine (3D Engine in C)
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#overview">About The Project</a>
      <ul>
        <li><a href="#features">Features</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#camera_controls">Camera Controls</a></li>
        <li><a href="#object_management">Object Management</a></li>
        <li><a href="#other_controls">Other Controls</a></li>
      </ul>
    </li>
    <li>
      <a href="#modules">Modules</a>
      <ul>
        <li><a href="rendering">Rendering</a></li>
        <li><a href="materials_and_textures">Materials and Textures</a></li>
        <li><a href="model_loading">Model Loading</a></li>
        <li><a href="object_management">Object Management</a></li>
        <li><a href="camera_and_vectors">Camera and Vectors</a></li>
        <li><a href="resource_loading">Resource Loading</a></li>
        <li><a href="user_interface">User Interface</a></li>
      </ul>
      <a href="#photo">Example usage of the Engine</a>
  </ol>
</details>

## Overview
C1ue Engine is a 3D rendering engine developed for creating and managing 3D objects and scenes. This engine leverages OpenGL for rendering, GLFW for window management, and Nuklear for the graphical user interface. It supports a wide range of features including various object types, texture management, PBR materials, camera control, and real-time rendering. :smile:

### Features
* Rendering: Utilizes OpenGL for efficient rendering of 3D objects.
* Materials and Textures: Supports PBR materials and texture management.
* Model Loading: Capable of loading and rendering 3D models.
* Object Management: Handles creation, updating, and deletion of 3D objects in the scene.
* Camera Control: Provides camera manipulation for navigating the 3D space.
* Graphical User Interface: Integrates Nuklear for GUI elements.
* File Operations: Supports saving and loading game states, logging, and configuration settings.

## Usage
### Camera Controls
* W/A/S/D: Move forward/left/backward/right
* Space: Move up
* Left Shift: Move down
* Mouse Movement: Look around
* CTRL: Speed Up the Movement
### Object Management
* O: Create a plane
* C: Create a cube
* H: Create a pyramid
* K: Create a sphere
* B: Create a cylinder
### Other Controls
* T: Toggle textures
* L: Toggle colors
* J: Toggle shading
* Q: Toggle PBR materials
* P: Pause the Engine
* E: Exit the program
  
## Modules
### Rendering:
* rendering.c
* shaders.c
### Materials and Textures:
* materials.c
* textures.c
### Model Loading:
* ModelLoad.c
### Object Management:
* ObjectManager.c
### Camera and Vectors:
* Camera.c
* Vectors.c
### Resource Loading:
* resource_loader.c
### User Interface:
* gui.c

## Photo

<p align="center">
  <img width="1080" height="720" src="https://github.com/Klus3kk/3D-Engine-in-C/assets/93116510/5d7d7ee0-69e4-47eb-b4dd-53081c3f799c">
</p>

