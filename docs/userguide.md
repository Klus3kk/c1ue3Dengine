# User Guide for ClueEngine

## Overview

Welcome to the **ClueEngine** user guide! This document will walk you through how to use **ClueEngine** to create and manage 3D scenes, manipulate objects, apply materials, work with lighting, and navigate using the graphical user interface (GUI). Whether you’re a beginner or an experienced developer, this guide will help you make the most out of **ClueEngine**.

## Getting Started

After setting up **ClueEngine** by following the [Setup Guide](docs/setup.md), you can dive into creating 3D scenes with the engine’s GUI. Here, we will cover the key features of **ClueEngine**, including object creation, camera navigation, material management, lighting, and how to interact with the GUI to customize your scene.

## 1. **Creating Objects**

**ClueEngine** provides the ability to create various types of 3D objects, which you can then manipulate and place in your scene.

### Available Object Types

- **Cube**: A standard 3D cube object.
- **Sphere**: A 3D spherical object.
- **Pyramid**: A 3D pyramid shape.
- **Cylinder**: A 3D cylindrical object.
- **Plane**: A flat, infinite plane (ideal for the ground or background).
- **Model**: Import and place custom 3D models.

### How to Add Objects

1. **Using the GUI Object Menu**: Open the object menu using the following keyboard shortcuts:
   - `O` for Plane
   - `C` for Cube
   - `H` for Pyramid
   - `K` for Sphere
   - `B` for Cylinder
Or press `I` to import a custom 3D model from a file.

2. **Creating Lights**: Lights can be added in a similar way:
   - `P` for Point Light
   - `D` for Directional Light
   - `S` for Spotlight

Each object can be manipulated after creation using the GUI and keyboard commands.

## 2. **Manipulating Objects**

Once you've added objects to the scene, you can transform them (move, rotate, scale) to suit your needs. Objects can be manipulated either using the GUI or keyboard shortcuts.

### Transformation Controls

- **Move Object**:

  - `W` to move forward
  - `A` to move left
  - `S` to move backward
  - `D` to move right
- **Rotate Object**: Use the mouse or keyboard to rotate the object.
- **Scale Object**: Use the scaling options in the GUI to resize an object.

You can also adjust the **material**, **color**, and **texture** of any selected object using the **Inspector Window**.

## 3. **Camera and Navigation**

Navigating through your scene is easy with **ClueEngine**'s camera controls. The engine supports first-person camera movement, allowing for intuitive navigation.

### Camera Movement

- **W**: Move forward
- **A**: Move left
- **S**: Move backward
- **D**: Move right
- **Space**: Move up
- **Left Shift**: Move down
- **Mouse Movement**: Look around
- **Left Click + Left Alt**: Drag the camera (when engine is paused)
- **Right Click**: Pan the camera (when engine is paused)

### Camera Controls in the GUI

You can adjust the camera's position, movement speed, and sensitivity in the **Settings Window**. This allows for fine control over how you navigate through the scene.

## 4. **Working with Materials**

**ClueEngine** supports both basic textures and **Physically-Based Rendering (PBR)** materials, enabling you to create highly realistic scenes.

### Applying Materials

1. **Default Materials**: By default, objects come with basic materials. You can change these through the **Inspector Window**.
2. **PBR Materials**: For more realistic rendering, apply **PBR materials** that simulate how light interacts with surfaces. This is done by selecting the **Use PBR** checkbox in the **Inspector Window**.

### Textures

Textures define how the surface of an object looks. You can assign textures to objects in a few simple steps:

1. Select the object from the scene.
2. In the **Inspector**, choose the **Change Texture** option and pick from the available textures.
3. To import custom textures, simply add them to your project and apply them via the **Inspector Window**.

## 5. **Lighting and Shadows**

Lighting plays a crucial role in creating realistic 3D environments. **ClueEngine** supports several types of lights to achieve various effects.

### Adding Lights

1. **Point Light**: A light that radiates in all directions from a point in space.
2. **Directional Light**: A light that shines in one direction, simulating sunlight.
3. **Spotlight**: A focused light source, often used for effects like flashlights.

Each light source has adjustable properties such as **position**, **intensity**, and **color**, which can be configured through the **Inspector Window**.

## 6. **Using the GUI**

**ClueEngine** integrates **Nuklear GUI**, which provides all the tools you need to manage your scene, adjust settings, and interact with objects in the engine.

### GUI Components

- **Object Menu**: For adding and managing objects in the scene.
- **Inspector Window**: View and modify the properties of selected objects.
- **Settings Window**: Adjust engine settings like resolution, camera speed, and more.
- **Console**: Displays debug information and engine status messages.

### GUI Controls

- **F1**: Toggle between light and dark theme for the GUI.
- **Ctrl + Z**: Undo the last action.
- **Ctrl + Y**: Redo the last action.
- **Ctrl + X**: Cut the selected object.
- **Ctrl + C**: Copy the selected object.
- **Ctrl + V**: Paste the object.
- **Delete**: Remove the selected object from the scene.

The GUI allows you to interact with **ClueEngine** seamlessly, providing an intuitive way to control and customize your 3D scenes.

## Conclusion

Now that you've got the basics down, you’re ready to begin building your own 3D scenes in **ClueEngine**! Whether you're creating games, simulations, or other 3D applications, this engine provides all the tools you need to manage your objects, apply materials, control lighting, and navigate through your scene with ease.

For further assistance, refer to other documents in the **docs** folder, such as [Engine Architecture](docs/EngineArchitecture.md) or [Extending the Engine](docs/ExtendingTheEngine.md), to dive deeper into **ClueEngine's** features and future capabilities.
