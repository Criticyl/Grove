#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Renderer/Shader.h"
#include "Renderer/VAO.h"
#include "Renderer/VBO.h"
#include "Renderer/EBO.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    if (!glfwInit()) {
        std::cerr << "[ERROR] Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Engine", NULL, NULL);
    if (window == NULL) {
        std::cerr << "[ERROR] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TEMPORARY FOR TESTING
    std::vector<float> vertices = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    std::vector<uint32_t> indices = {
        0, 1, 3,
        1, 2, 3
    };

    Shader shaderProg("resources/shaders/default.vert", "resources/shaders/default.frag");
    VAO quadVAO;
    quadVAO.bind();

    VBO quadVBO(vertices);
    EBO quadEBO(indices);
    quadVAO.linkAttribute(quadVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    quadVAO.unbind();
    quadVBO.unbind();
    quadEBO.unbind();

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProg.use();
        quadVAO.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    quadVAO.deleteVAO();
    quadVBO.deleteBuffer();
    quadEBO.deleteBuffer();
    shaderProg.deleteProgram();

    glfwTerminate();
    return 0;
}