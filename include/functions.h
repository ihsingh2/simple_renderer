#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <limits>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

inline float sample_uniform() {
    return (float)(rand() / (RAND_MAX + 1.0));
}

// callback.cpp
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// init.cpp
void initializeGLFW();
GLFWwindow* createWindow();
void initializeOpenGL();

// render.cpp
int main(int argc, char* argv[]);

// shapes.cpp
void createShape(int num_sides);
void translateShape(glm::vec3 translation);
void rotateShapeX(float alpha);
void rotateShapeY(float alpha);
void rotateShapeZ(float alpha);

#endif
