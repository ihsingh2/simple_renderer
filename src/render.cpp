#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <constants.h>
#include <functions.h>
#include <shader.h>

bool showPrism = true;
std::vector<float> vertices_prism;
std::vector<unsigned int> indices_prism;
std::vector<float> vertices_pyramid;
std::vector<unsigned int> indices_pyramid;
glm::vec3 objectCenter = glm::vec3(0.0f, 0.0f, 0.0f);

float fov = 45.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -2.5f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
glm::mat4 projection = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: ./render <num_sides>" << std::endl;
        return 1;
    }
    int num_sides = atoi(argv[1]);
    if (num_sides < 3) {
        std::cerr << "bad argument: num_sides should be atleast 3." << std::endl;
        return 1;
    }

    initializeGLFW();
    GLFWwindow *window = createWindow();
    initializeOpenGL();

    Shader shader = Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    createShape(num_sides);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_prism.size() * sizeof(float), &vertices_prism.front(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_prism.size() * sizeof(unsigned int), &indices_prism.front(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cameraDirection = glm::normalize(cameraPos - cameraTarget);
        view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        if (showPrism) {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_prism.size() * sizeof(float), &vertices_prism.front(), GL_DYNAMIC_DRAW);
            glDrawElements(GL_TRIANGLES, indices_prism.size(), GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_prism.size() * sizeof(unsigned int), &indices_prism.front(), GL_DYNAMIC_DRAW);
        }
        else {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_pyramid.size() * sizeof(float), &vertices_pyramid.front(), GL_DYNAMIC_DRAW);
            glDrawElements(GL_TRIANGLES, indices_pyramid.size(), GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_pyramid.size() * sizeof(unsigned int), &indices_pyramid.front(), GL_DYNAMIC_DRAW);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}
