#pragma once

#include <glad/glad.h>
#include "VBO.h"

namespace Grove {
    class VAO {
    public:
        std::uint32_t ID;

    public:

        VAO();

        void linkAttribute(VBO& vbo, unsigned int layout, unsigned int numComponents, unsigned int type, int stride, void* offset);

        void bind();
        void unbind();
        void deleteVAO();
    };
}