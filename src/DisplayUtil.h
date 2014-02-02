#ifndef DISPLAYUTIL_H
#define DISPLAYUTIL_H

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern GLuint shaderProgram;

GLFWmonitor* getMonitorAtPosition(glm::ivec2 & position);
GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);
void InitProgram();

#endif /* DISPLAYUTIL_H */