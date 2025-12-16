#pragma once

#include <memory>

#include "Window.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Shader.h"
#include "../Renderer/VAO.h"
#include "../Renderer/VBO.h"
#include "../Renderer/EBO.h"
#include "../World/Chunk.h"


namespace Grove {

    class Application {
    public:
        Application();
        ~Application();

        void run();

    private:

        void onUpdate(float deltaTime);
        void onRender();
        void processInputs();

    private:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Camera> m_Camera;

        // Temp
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Chunk> m_TestChunk;

        float m_DeltaTime;
        float m_LastFrame;
    };


}