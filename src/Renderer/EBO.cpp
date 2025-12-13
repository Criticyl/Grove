#include "EBO.h"

namespace Grove {
    EBO::EBO(std::vector<std::uint32_t>& indices) {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
    }

    void EBO::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void EBO::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void EBO::deleteBuffer() {
        glDeleteBuffers(1, &ID);
    }
}