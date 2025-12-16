#include "Window.h"

namespace Grove {

    Window::Window(int width, int height, const std::string& title)
        : m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr)
    {
        m_LastMouseX = width / 2.0f;
        m_LastMouseY = height / 2.0f;
        m_FirstMouse = true;
        m_MouseOffsetX = 0.0f;
        m_MouseOffsetY = 0.0f;
    }

    Window::~Window() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
        }
        glfwTerminate();
    }

    void Window::init() {

        if (!glfwInit()) {
            std::cerr << "[ERROR] Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
        if (m_Window == NULL) {
            std::cerr << "[ERROR] Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_Window);

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetFramebufferSizeCallback(m_Window, s_framebufferSizeCallback);
        glfwSetCursorPosCallback(m_Window, s_mouseCallback);

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "[ERROR] Failed to initialize GLAD" << std::endl;
            return;
        }

        glEnable(GL_DEPTH_TEST);
    }

    void Window::update() {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::s_framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Window* instance = (Window*)glfwGetWindowUserPointer(window);
        if (instance) {
            instance->onResize(width, height);
        }
    }

    void Window::s_mouseCallback(GLFWwindow* window, double xPos, double yPos) {
        Window* instance = (Window*)glfwGetWindowUserPointer(window);
        if (instance) {
            instance->onMouseMove(xPos, yPos);
        }
    }

    void Window::onResize(int width, int height) {
        m_Width = width;
        m_Height = height;
        glViewport(0, 0, width, height);
    }

    void Window::onMouseMove(double xPos, double yPos) {
        if (m_FirstMouse) {
            m_LastMouseX = xPos;
            m_LastMouseY = yPos;
            m_FirstMouse = false;
        }

        float xOffset = xPos - m_LastMouseX;
        float yOffset = m_LastMouseY - yPos;

        m_LastMouseX = xPos;
        m_LastMouseY = yPos;

        m_MouseOffsetX = xOffset;
        m_MouseOffsetY = yOffset;
    }

}