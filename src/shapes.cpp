#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <constants.h>
#include <functions.h>

extern std::vector<float> vertices_prism;
extern std::vector<unsigned int> indices_prism;
extern std::vector<float> vertices_pyramid;
extern std::vector<unsigned int> indices_pyramid;
extern glm::vec3 objectCenter;
// extern glm::vec3 cameraTarget;

void createShape(int num_sides) {
    float radius = 0.5;
    float delta = 2 * M_PI / num_sides;
    float theta_0 = (- M_PI / 2) + (delta / 2);

    srand(time(NULL));

    for (int i = 0; i < num_sides; i++) {
        vertices_prism.push_back(radius * std::cos(theta_0 + delta * i));
        vertices_prism.push_back(radius * std::sin(theta_0 + delta * i));
        vertices_prism.push_back(-1.0f);
        vertices_prism.push_back(sample_uniform());
        vertices_prism.push_back(sample_uniform());
        vertices_prism.push_back(sample_uniform());

        vertices_pyramid.push_back(radius * std::cos(theta_0 + delta * i));
        vertices_pyramid.push_back(radius * std::sin(theta_0 + delta * i));
        vertices_pyramid.push_back(-1.0f);
        vertices_pyramid.push_back(sample_uniform());
        vertices_pyramid.push_back(sample_uniform());
        vertices_pyramid.push_back(sample_uniform());
    }

    for (int i = 0; i < num_sides; i++) {
        vertices_prism.push_back(radius * std::cos(theta_0 + delta * i));
        vertices_prism.push_back(radius * std::sin(theta_0 + delta * i));
        vertices_prism.push_back(1.0f);
        vertices_prism.push_back(sample_uniform());
        vertices_prism.push_back(sample_uniform());
        vertices_prism.push_back(sample_uniform());
    }

    vertices_pyramid.push_back(0.0f);
    vertices_pyramid.push_back(0.0f);
    vertices_pyramid.push_back(1.0f);
    vertices_pyramid.push_back(sample_uniform());
    vertices_pyramid.push_back(sample_uniform());
    vertices_pyramid.push_back(sample_uniform());

    for (unsigned int i = 1; i < num_sides - 1; i++) {
        indices_prism.push_back(0);
        indices_prism.push_back(i);
        indices_prism.push_back(i + 1);

        indices_pyramid.push_back(0);
        indices_pyramid.push_back(i);
        indices_pyramid.push_back(i + 1);
    }

    for (unsigned int i = 1; i < num_sides - 1; i++) {
        indices_prism.push_back(num_sides);
        indices_prism.push_back(num_sides + i);
        indices_prism.push_back(num_sides + i + 1);
    }

    for (unsigned int i = 0; i < num_sides; i++) {
        indices_prism.push_back(i);
        indices_prism.push_back(num_sides + i);
        indices_prism.push_back(num_sides + ((i + 1) % num_sides));
        indices_prism.push_back(i);
        indices_prism.push_back((i + 1) % num_sides);
        indices_prism.push_back(num_sides + ((i + 1) % num_sides));

        indices_pyramid.push_back(i);
        indices_pyramid.push_back((i + 1) % num_sides);
        indices_pyramid.push_back(num_sides);
    }
}

void translateShape(glm::vec3 translation) {
    objectCenter += translation;
    // cameraTarget += translation;
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        vertices_prism[i] += translation[0];
        vertices_prism[i + 1] += translation[1];
        vertices_prism[i + 2] += translation[2];
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        vertices_pyramid[i] += translation[0];
        vertices_pyramid[i + 1] += translation[1];
        vertices_pyramid[i + 2] += translation[2];
    }
}

void rotateShapeX() {
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        vertices_prism[i + 1] = vertices_prism[i + 1] * std::cos(ROT_ANGLE) - vertices_prism[i + 2] * std::sin(ROT_ANGLE);
        vertices_prism[i + 2] = vertices_prism[i + 1] * std::sin(ROT_ANGLE) + vertices_prism[i + 2] * std::cos(ROT_ANGLE);
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        vertices_pyramid[i + 1] = vertices_pyramid[i + 1] * std::cos(ROT_ANGLE) - vertices_pyramid[i + 2] * std::sin(ROT_ANGLE);
        vertices_pyramid[i + 2] = vertices_pyramid[i + 1] * std::sin(ROT_ANGLE) + vertices_pyramid[i + 2] * std::cos(ROT_ANGLE);
    }
}

void rotateShapeY() {
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        vertices_prism[i] = vertices_prism[i] * std::cos(ROT_ANGLE) + vertices_prism[i + 2] * std::sin(ROT_ANGLE);
        vertices_prism[i + 2] = vertices_prism[i + 2] * std::cos(ROT_ANGLE) - vertices_prism[i] * std::sin(ROT_ANGLE);
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        vertices_pyramid[i] = vertices_pyramid[i] * std::cos(ROT_ANGLE) + vertices_pyramid[i + 2] * std::sin(ROT_ANGLE);
        vertices_pyramid[i + 2] = vertices_pyramid[i + 2] * std::cos(ROT_ANGLE) - vertices_pyramid[i] * std::sin(ROT_ANGLE);
    }
}

void rotateShapeZ() {
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        vertices_prism[i] = vertices_prism[i] * std::cos(ROT_ANGLE) - vertices_prism[i + 1] * std::sin(ROT_ANGLE);
        vertices_prism[i + 1] = vertices_prism[i] * std::sin(ROT_ANGLE) + vertices_prism[i + 1] * std::cos(ROT_ANGLE);
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        vertices_pyramid[i] = vertices_pyramid[i] * std::cos(ROT_ANGLE) - vertices_pyramid[i + 1] * std::sin(ROT_ANGLE);
        vertices_pyramid[i + 1] = vertices_pyramid[i] * std::sin(ROT_ANGLE) + vertices_pyramid[i + 1] * std::cos(ROT_ANGLE);
    }
}
