#include "Shader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


const std::string LOCAL_FILE_DIR("data/");
// Vertex Shader source code

std::string LoadShaderCodeOrThrow(const std::string &strBasename, const std::string &program_id) {
    std::string strFilename = LOCAL_FILE_DIR + program_id + '/' + strBasename;
    std::ifstream shaderFile(strFilename);
    if (!shaderFile.is_open()) {
        std::cerr << "Could not find the file " + strBasename + " for program with id : " + program_id +
                "(at location :" + strFilename + ")";
        exit(1);
    }

    return {
        (std::istreambuf_iterator<char>(shaderFile)),
        std::istreambuf_iterator<char>()
    };
}

// Function to compile shaders
unsigned int compileShader(unsigned int type, const std::string &source) {
    const unsigned int id = glCreateShader(type);
    auto source_c_str = source.c_str();
    glShaderSource(id, 1, &source_c_str, nullptr);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        GLchar *infoLog = new GLchar[length];
        glGetShaderInfoLog(id, length, nullptr, infoLog);
        std::cerr << "Shader compilation failed\n" << infoLog << std::endl;
        delete [] infoLog;
    }

    return id;
}

GLuint LoadShader(const GLenum eShaderType, const std::string &strShaderFilename, const std::string &programId) {
    const std::string shader_data = LoadShaderCodeOrThrow(strShaderFilename, programId);
    return compileShader(eShaderType, shader_data);
}


// Function to link vertex and fragment shaders into a program
unsigned int InitializeProgram(const std::string &program_id) {
    std::vector<GLuint> shaders;

    shaders.push_back(LoadShader(GL_FRAGMENT_SHADER, "frag.frag", program_id));
    shaders.push_back(LoadShader(GL_VERTEX_SHADER, "vert.vert", program_id));
    GLuint shaderProgram = glCreateProgram();

    std::ranges::for_each(shaders, [shaderProgram](const GLuint &s_id) {
        glAttachShader(shaderProgram, s_id);
    });
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLint length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        GLchar *infoLog = new GLchar[length];
        glGetProgramInfoLog(shaderProgram, length, nullptr, infoLog);
        std::cerr << "Shader linking failed\n" << infoLog << std::endl;
        delete[] infoLog;
    }

    std::ranges::for_each(shaders,glDeleteShader);

    return shaderProgram;
}
