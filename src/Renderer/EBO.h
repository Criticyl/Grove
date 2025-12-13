#pragma once

#include <glad/glad.h>
#include <vector>

namespace Grove {

    class EBO {
    public:
        std::uint32_t ID;

    public:

        EBO(std::vector<std::uint32_t>& indices);

        void bind();
        void unbind();
        void deleteBuffer();
    };
}