#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <constants.h>
#include <functions.h>

extern bool showPrism;
extern glm::vec3 objectCenter;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraTarget;
extern glm::vec3 cameraUp;

void processInput(GLFWwindow *window) {
    // Camera translation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)           // forward
        cameraPos += CAM_SPEED * glm::normalize(cameraTarget - cameraPos);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)           // backward
        cameraPos -= CAM_SPEED * glm::normalize(cameraTarget - cameraPos);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)           // left
        cameraPos -= CAM_SPEED * (glm::cross(glm::normalize(cameraTarget - cameraPos), cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)           // right
        cameraPos += CAM_SPEED * (glm::cross(glm::normalize(cameraTarget - cameraPos), cameraUp));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)           // up
        cameraPos += CAM_SPEED * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)           // down
        cameraPos -= CAM_SPEED * cameraUp;

    // Object translation
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)           // forward
        translateShape(OBJ_SPEED * glm::normalize(cameraPos - objectCenter));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)           // backward
        translateShape(OBJ_SPEED * glm::normalize(objectCenter - cameraPos));
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)           // left
        translateShape(OBJ_SPEED * (glm::cross(glm::normalize(cameraPos - objectCenter), cameraUp)));
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)           // right
        translateShape(OBJ_SPEED * (glm::cross(glm::normalize(objectCenter - cameraPos), cameraUp)));
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)       // up
        translateShape(OBJ_SPEED * cameraUp);
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)      // down
        translateShape(OBJ_SPEED * - cameraUp);

    // Object rotation
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        glm::vec3 center = objectCenter;
        translateShape(-center);
        rotateShapeY(ROT_ANGLE);
        translateShape(center);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Toggle shape
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        showPrism = !showPrism;
    }

    // Re-center camera
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        cameraTarget = objectCenter;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
