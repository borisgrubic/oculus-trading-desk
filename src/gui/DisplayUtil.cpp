#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "DisplayUtil.h"

#include "ScreenModel.h"

GLuint screenShaderProgram;
GLuint visAShaderProgram;



// Get the 'monitor' at position specified by HMD Info
// This should be the OVR
GLFWmonitor* getMonitorAtPosition(glm::ivec2 & position) {
	int count;
	GLFWmonitor ** monitors = glfwGetMonitors(&count);
	for (int i = 0; i < count; ++i) {
		glm::ivec2 candidatePosition;
		glfwGetMonitorPos(monitors[i], &candidatePosition.x, &candidatePosition.y);
		if (candidatePosition == position) {
			return monitors[i];
		}
	}
	return nullptr;
}

// Compile string to OpenGL shader
GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile){

	GLuint shader = glCreateShader(eShaderType);

	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	} else {
		std::cout << "Shader loaded. " << std::endl;
	}

	return shader;

}

// Create program from a vector of shaders
GLuint CreateProgram(const std::vector<GLuint> &shaderList){

	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;

}

// Initialise the screen display program with shaders
void InitProgram(){
    
        // Screen shader program
	std::vector<GLuint> screenShaderList;
	screenShaderList.push_back(CreateShader(GL_VERTEX_SHADER, screenVertexShader));
	screenShaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, screenFragmentShader));
	screenShaderProgram = CreateProgram(screenShaderList);
	std::for_each(screenShaderList.begin(), screenShaderList.end(), glDeleteShader);
        
        std::cout << "screen program loaded" << std::endl;

}