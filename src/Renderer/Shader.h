#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:

    std::uint32_t ID;

public:

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void deleteProgram();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    void checkCompileErrors(std::uint32_t shader, std::string type);
};