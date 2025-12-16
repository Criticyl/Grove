#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

namespace Grove {
    class Window {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void init();
        void update();

        bool shouldClose() const { return glfwWindowShouldClose(m_Window); }

        GLFWwindow* getNativeWindow() const { return m_Window; }
        int getWidth() const { return m_Width; }
        int getHeight() const { return m_Height; }

        float getMouseXOffset() {
            float temp = m_MouseOffsetX;
            m_MouseOffsetX = 0.0f;
            return temp;
        }

        float getMouseYOffset() {
            float temp = m_MouseOffsetY;
            m_MouseOffsetY = 0.0f;
            return temp;
        }

    private:

        static void s_framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void s_mouseCallback(GLFWwindow* window, double xPos, double yPos);

    private:

        void onResize(int width, int height);
        void onMouseMove(double xPos, double yPos);

    private:

        GLFWwindow* m_Window;
        int m_Width, m_Height;
        std::string m_Title;

        float m_LastMouseX, m_LastMouseY;
        float m_MouseOffsetX, m_MouseOffsetY;
        bool m_FirstMouse;

    };
}