#include "Application.h"
#include "../Renderer/EBO.h"

namespace Grove {

    Application::Application()
        : m_DeltaTime(0.0f), m_LastFrame(0.0f)
    {
        m_Window = std::make_unique<Window>(1280, 720, "Grove");
        m_Window->init();

        m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Shader = std::make_unique<Shader>("resources/shaders/default.vert", "resources/shaders/default.frag");

        std::vector<float> vertices = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };

        std::vector<std::uint32_t> indices = {
            0, 1, 3,
            1, 2, 3
        };

        m_VAO = std::make_unique<VAO>();
        m_VAO->bind();

        m_VBO = std::make_unique<VBO>(vertices);
        m_EBO = std::make_unique<EBO>(indices);

        m_VAO->linkAttribute(*m_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        m_VAO->unbind();
        m_VBO->unbind();
        m_EBO->unbind();
    }

    Application::~Application() {

    }

    void Application::run() {

        while (!(m_Window->shouldClose())) {
            float currentFrame = static_cast<float>(glfwGetTime());
            m_DeltaTime = currentFrame - m_LastFrame;
            m_LastFrame = currentFrame;

            processInputs();
            onUpdate(m_DeltaTime);
            onRender();

            m_Window->update();
        }
    }

    void Application::processInputs() {
        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window->getNativeWindow(), true);

        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_Camera->processKeyboard(FORWARD, m_DeltaTime);
        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_Camera->processKeyboard(BACKWARD, m_DeltaTime);
        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_Camera->processKeyboard(LEFT, m_DeltaTime);
        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_Camera->processKeyboard(RIGHT, m_DeltaTime);

        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            m_Camera->processKeyboard(UP, m_DeltaTime);
        if (glfwGetKey(m_Window->getNativeWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            m_Camera->processKeyboard(DOWN, m_DeltaTime);

        float xOffset = m_Window->getMouseXOffset();
        float yOffset = m_Window->getMouseYOffset();
        m_Camera->processMouseMovement(xOffset, yOffset);
    }

    void Application::onUpdate(float deltaTime) {
        //Game ticking stuff
    }

    void Application::onRender() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader->use();

        glm::mat4 projection = glm::perspective(glm::radians(m_Camera->cameraFOV), (float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 100.0f);
        glm::mat4 view = m_Camera->getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        m_Shader->setMat4("projection", projection);
        m_Shader->setMat4("view", view);
        m_Shader->setMat4("model", model);

        m_VAO->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}