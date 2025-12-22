#include "VBO.h"

namespace Grove {
    VBO::VBO(std::vector<float>& vertices) {

        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    }

    VBO::~VBO() {
        glDeleteBuffers(1, &ID);
    }

    void VBO::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    void VBO::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VBO::deleteBuffer() {
        glDeleteBuffers(1, &ID);
    }
}