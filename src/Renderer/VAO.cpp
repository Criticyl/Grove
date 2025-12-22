#include "VAO.h"

namespace Grove {
    VAO::VAO() {
        glGenVertexArrays(1, &ID);
    }

    VAO::~VAO() {
        glDeleteVertexArrays(1, &ID);
    }

    void VAO::linkAttribute(VBO& vbo, unsigned int layout, unsigned int numComponents, unsigned int type, int stride, void* offset) {
        vbo.bind();

        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        vbo.unbind();
    }

    void VAO::bind() {
        glBindVertexArray(ID);
    }

    void VAO::unbind() {
        glBindVertexArray(0);
    }

    void VAO::deleteVAO() {
        glDeleteVertexArrays(1, &ID);
    }
}