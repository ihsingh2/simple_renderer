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

void rotateShapeX(float alpha) {
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        float y = vertices_prism[i + 1];
        float z = vertices_prism[i + 2];
        vertices_prism[i + 1] = y * std::cos(alpha) - z * std::sin(alpha);
        vertices_prism[i + 2] = y * std::sin(alpha) + z * std::cos(alpha);
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        float y = vertices_prism[i + 1];
        float z = vertices_prism[i + 2];
        vertices_pyramid[i + 1] = y * std::cos(alpha) - z * std::sin(alpha);
        vertices_pyramid[i + 2] = y * std::sin(alpha) + z * std::cos(alpha);
    }
}

void rotateShapeY(float alpha) {
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        float x = vertices_prism[i];
        float z = vertices_prism[i + 2];
        vertices_prism[i] = x * std::cos(alpha) + z * std::sin(alpha);
        vertices_prism[i + 2] = z * std::cos(alpha) - x * std::sin(alpha);
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        float x = vertices_pyramid[i];
        float z = vertices_pyramid[i + 2];
        vertices_pyramid[i] = x * std::cos(alpha) + z * std::sin(alpha);
        vertices_pyramid[i + 2] = z * std::cos(alpha) - x * std::sin(alpha);
    }
}

void rotateShapeZ(float alpha) {
    for (int i = 0; i < vertices_prism.size(); i = i + 6) {
        float x = vertices_prism[i];
        float y = vertices_prism[i + 1];
        vertices_prism[i] = x * std::cos(alpha) - y * std::sin(alpha);
        vertices_prism[i + 1] = x * std::sin(alpha) + y * std::cos(alpha);
    }
    for (int i = 0; i < vertices_pyramid.size(); i = i + 6) {
        float x = vertices_prism[i];
        float y = vertices_prism[i + 1];
        vertices_pyramid[i] = x * std::cos(alpha) - y * std::sin(alpha);
        vertices_pyramid[i + 1] = x * std::sin(alpha) + y * std::cos(alpha);
    }
}
