#pragma once

#include <glad/glad.h>
#include <vector>

class VBO {
public:
    std::uint32_t ID;

public:

    VBO(std::vector<float>& vertices);

    void bind();
    void unbind();
    void deleteBuffer();

};
