# ClueEngine Architecture

## Overview

**ClueEngine** is structured around a modular design that enables easy extension and customization. The engine consists of several key modules that interact to provide real-time 3D rendering, object management, camera controls, and user interface capabilities. The following sections describe each core component of the engine.

## Core Modules

### 1. **Main Loop**

At the heart of **ClueEngine** is the **main loop**, which runs continuously while the engine is active. This loop handles the rendering process, input events, updates to the scene, and interactions with the graphical user interface (GUI). 

The main loop is designed to be efficient, ensuring that the frame rate remains steady while managing resources and ensuring smooth user interaction.

### 2. **Rendering**

Rendering is powered by **OpenGL**, with the engine supporting shaders, materials, and textures. The rendering system is broken down into the following components:

#### 2.1 **Shaders**

The engine uses both **vertex** and **fragment** shaders to process the graphics pipeline. These shaders are written in GLSL and are responsible for handling lighting, texture mapping, and material properties. The shaders are compiled and linked into programs, which are then used to render objects to the screen.

#### 2.2 **Textures and Materials**

The engine supports both basic textures and PBR materials. PBR materials are designed to offer realistic lighting and reflections by simulating real-world material properties such as roughness, metallicity, and ambient occlusion.

Textures are managed using **SOIL2**, and the engine supports a variety of texture formats, including images for diffuse, specular, and normal maps.

#### 2.3 **Lighting**

The engine supports multiple types of lighting:

- **Point Lights**: Lights that emit in all directions from a single point in space.
- **Directional Lights**: Lights that simulate sunlight and cast parallel rays in one direction.
- **Spotlights**: Lights that cast a cone of light in a specified direction.

Each type of light has its own unique behavior in terms of attenuation and the way it interacts with objects in the scene.

### 3. **Camera and Controls**

The camera system allows for first-person navigation of the 3D space, supporting basic movements such as **WASD** for translation and **mouse movements** for looking around. The camera can be zoomed and panned using the mouse, and the camera settings (e.g., speed and sensitivity) can be adjusted through the GUI.

The camera is represented as an object with position, orientation, and projection matrix, which is used to transform 3D world coordinates into 2D screen space for rendering.

### 4. **Object Management**

Objects in the scene are represented as **SceneObject** instances. Each object can be a 3D shape (e.g., cube, sphere, pyramid, cylinder) or a model (e.g., imported from external files). 

#### 4.1 **Creating Objects**

Objects can be added to the scene using specific functions, and each object has properties that can be modified:

- **Position**: The object's location in 3D space.
- **Rotation**: The object's orientation.
- **Scale**: The size of the object.
- **Material/Texture**: The surface properties of the object.

#### 4.2 **Object Transformation**

The engine supports transformations for each object, including translation, rotation, and scaling. These transformations are applied in local space, and can be dynamically modified based on user input or logic defined in the application.

### 5. **User Interface (UI)**

The **ClueEngine** includes an integrated graphical user interface powered by **Nuklear**. The UI allows the user to interact with the engine's features, such as adding objects, adjusting materials, and controlling camera settings. 

The interface includes multiple panels, including:

- **Settings**: For adjusting camera speed, resolution, background color, etc.
- **Object Controls**: For adding, removing, and transforming objects in the scene.
- **Lighting Controls**: For enabling and adjusting lights in the scene.
- **Debug Info**: Displays performance and error information.

The GUI is designed to be minimal and intuitive, offering powerful controls without overwhelming the user.

### 6. **Resource Management**

Resources such as textures, models, and shaders are managed by dedicated systems to ensure that they are loaded efficiently and used appropriately. 

- **Textures** are loaded using **SOIL2** and stored in memory, accessible by the objects that require them.
- **Models** can be loaded from files and stored as meshes for rendering.
- **Shaders** are compiled and linked when needed and are cached for performance.

## Workflow

The engine's core workflow involves several key steps:

1. **Initialization**: Upon startup, the engine initializes all subsystems, loads resources, and sets up the window, OpenGL context, and GUI.
2. **Main Loop**: The engine enters the main loop, where input events are processed, objects are updated, and the scene is rendered.
3. **Rendering**: The engine prepares the shaders, applies transformations to the objects, and renders them to the screen based on the camera's perspective.
4. **Post-Processing**: After rendering the scene, any post-processing effects (e.g., lighting adjustments) are applied.
5. **User Interaction**: The UI allows users to interact with objects and adjust settings in real-time.

## Extending the Engine

The modular design of **ClueEngine** allows for easy extension. Developers can add new object types, modify the camera system, or even integrate custom shaders. The **Object Management** and **Camera** modules are particularly easy to extend, making it simple to add new features and functionality.

## Conclusion

**ClueEngine** provides a powerful and flexible framework for developing 3D applications, with a focus on real-time rendering, camera control, and a seamless user interface. The modular architecture ensures that the engine can be easily extended and customized for different types of 3D applications, whether for games, simulations, or other interactive environments.
