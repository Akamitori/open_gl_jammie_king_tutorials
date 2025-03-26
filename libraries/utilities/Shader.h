#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "export.h"

EXPORTED std::string LoadShaderCodeOrThrow(const std::string &strBasename, const std::string &program_id);
EXPORTED GLuint LoadShader(const GLenum eShaderType, const std::string &strShaderFilename, const std::string &programId);
EXPORTED unsigned int InitializeProgram(const std::string &program_id);
#endif //SHADER_H
