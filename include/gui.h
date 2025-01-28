#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "globals.h"
#include <glad/glad.h>
#include "ObjectManager.h"
#include "SceneObject.h"
#include "ModelLoad.h"
#define MAX_VERTEX_BUFFER 1024 * 1024
#define MAX_ELEMENT_BUFFER 1024 * 1024

extern SceneObject scene_objects[MAX_OBJECTS];
extern int num_scene_objects;
extern GLuint textureColorbuffer;
const char* objectTypeName(ObjectType type);

extern GLFWwindow* window;
extern bool theme_dark;
extern bool show_about;
extern bool is_fullscreen;
extern int windowed_x, windowed_y, windowed_width, windowed_height;
void change_texture(SceneObject* obj);
void setup_nuklear(GLFWwindow* existingWindow);
void set_theme(bool dark_theme);
void toggle_theme();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void main_gui();
void resize_callback(GLFWwindow* window, int width, int height);
void teardown_nuklear();
void toggle_object_property(SceneObject* obj, const char* property);
void render_nuklear();
void run_loading_screen(GLFWwindow* window, struct nk_context* ctx);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
struct nk_context* get_nk_context();

#endif 
