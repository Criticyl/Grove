#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vertShaderStream, fragShaderStream;

        vertShaderStream << vertexFile.rdbuf();
        fragShaderStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertShaderStream.str();
        fragmentCode = fragShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "[ERROR: Shader] File could not be read: " << e.what() << std::endl;
    }

    std::uint32_t vertShader, fragShader;
    
    const char* c_vertexCode = vertexCode.c_str();
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &c_vertexCode, NULL);
    glCompileShader(vertShader);
    checkCompileErrors(vertShader, "VERTEX");

    const char* c_fragmentCode = fragmentCode.c_str();
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &c_fragmentCode, NULL);
    glCompileShader(fragShader);
    checkCompileErrors(fragShader, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::deleteProgram() {
    glDeleteProgram(ID);
}

// --------------------------- Uniforms --------------------------------
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

// ---------------------------------------------------------------------

void Shader::checkCompileErrors(std::uint32_t shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "[ERROR: Shader] Failed to compile " << type << " shader: \n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "[ERROR: Shader Program] Failed to link program: \n" << infoLog << std::endl;
        }
    }
}