#include <string>
#include <GL/glew.h>
#include "ScreenModel.h"

// Screen Vertex shader
const std::string screenVertexShader =
"#version 330\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec4 color;\n"
"smooth out vec4 theColor;\n"
"uniform mat4 projectionMat;\n"
"uniform mat4 modelViewMat;\n"
"void main(){\n"
"gl_Position = projectionMat * modelViewMat * position; \n"
"theColor = color;\n"
	"}\n";

// Screen Fragment shader
const std::string screenFragmentShader =
"#version 330\n"
"smooth in vec4 theColor;\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"   outputColor = theColor;\n"
	"}\n";

// VisualisationA Vertex shader
const std::string visAVertexShader =
"#version 330\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec4 color;\n"
"smooth out vec4 theColor;\n"
"uniform mat4 projectionMat;\n"
"uniform mat4 modelViewMat;\n"
"uniform vec4 offset;\n"
"uniform vec4 scale;\n"
"uniform vec4 recolor;\n"
"void main(){\n"
"gl_Position = projectionMat * modelViewMat * ((position*scale) + offset); \n"
"theColor = recolor;\n"
	"}\n";

// VisualisationA Fragment shader
const std::string visAFragmentShader =
"#version 330\n"
"smooth in vec4 theColor;\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"   outputColor = theColor;\n"
	"}\n";


/*
	Trading Desk Screens
			TOP
	LEFT	MAIN	RIGHT
		   BOTTOM
*/

// Screen buffers
GLuint mainScreenBuffer;
GLuint topScreenBuffer;
GLuint bottomScreenBuffer;
GLuint leftScreenBuffer;
GLuint rightScreenBuffer;

GLuint testGraphBuffer;

GLuint visA1Buffer;
GLuint visA2Buffer;
GLuint visA3Buffer;

// Main screen (center)
const float mainScreen[] = {
	// position
	3.5f, 1.5f, -3.0f, 1.0f,
	3.5f, -1.5f, -3.0f, 1.0f,
	-3.5f, -1.5f, -3.0f, 1.0f,
	-3.5f, 1.5f, -3.0f, 1.0f,
	// color
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

const float topScreen[] = {
	// position
	4.0f, 1.5f, -3.0f, 1.0f,
	4.0f, 6.0f, 0.0f, 1.0f,
	-4.0f, 6.0f, 0.0f, 1.0f,
	-4.0f, 1.5f, -3.0f, 1.0f,
	// color
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

const float bottomScreen[] = {
	// position
	-4.0f, -1.5f, -3.0f, 1.0f,
	-4.0f, -6.0f, 0.0f, 1.0f,
	4.0f, -6.0f, 0.0f, 1.0f,
	4.0f, -1.5f, -3.0f, 1.0f,
	// color
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

const float leftScreen[] = {
	// position
	3.5f, 4.0f, -3.0f, 1.0f,
	6.0f, 4.0f, 5.0f, 1.0f,
	6.0f, -4.0f, 5.0f, 1.0f,
	3.5f, -4.0f, -3.0f, 1.0f,
	// color
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

const float rightScreen[] = {
	// position
	-3.5f, 4.0f, -3.0f, 1.0f,
	-6.0f, 4.0f, 5.0f, 1.0f,
	-6.0f, -4.0f, 5.0f, 1.0f,
	-3.5f, -4.0f, -3.0f, 1.0f,
	// color
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

const float testGraph[] = {
    
        // Box 1
	// position
	3.0f, 1.0f, -2.9f, 1.0f,
	3.0f, -1.0f, -2.9f, 1.0f,
	2.0f, -1.0f, -2.9f, 1.0f,
	2.0f, 1.0f, -2.9f, 1.0f,
	// color
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
    
    
};

// First visualisation
// TODO: Move into its own file
const float visA1[] = {
    
        // Centre divisor
	// position
	0.01f, 1.4f, -2.9f, 1.0f,
	0.01f, -1.4f, -2.9f, 1.0f,
	-0.01f, -1.4f, -2.9f, 1.0f,
	-0.01f, 1.4f, -2.9f, 1.0f,
	// color
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
        // Box 1
};
const float visA2[] = {
        // position
	0.0f, 1.4f, -2.9f, 1.0f,
	0.0f, 1.2f, -2.9f, 1.0f,
	1.0f, 1.2f, -2.9f, 1.0f,
	1.0f, 1.4f, -2.9f, 1.0f,
	// color
	0.8f, 0.2f, 0.0f, 1.0f,
	0.8f, 0.2f, 0.0f, 1.0f,
	0.8f, 0.0f, 0.0f, 1.0f,
	0.8f, 0.0f, 0.0f, 1.0f, 
};

// Fill the buffers with vertex data for each screen
void InitBuffers(){

	glGenBuffers(1, &mainScreenBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mainScreenBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mainScreen), mainScreen, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &topScreenBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, topScreenBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topScreen), topScreen, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &bottomScreenBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bottomScreenBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottomScreen), bottomScreen, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &leftScreenBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, leftScreenBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftScreen), leftScreen, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &rightScreenBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rightScreenBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightScreen), rightScreen, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
       

}

void InitTestGraphBuffers(){
    
    std::cout << "Test graph buffers initalised" << std::endl;
        
    	glGenBuffers(1, &testGraphBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, testGraphBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testGraph), testGraph, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void InitVisualisationBuffers(){
        
    // Visualisation 1
    	glGenBuffers(1, &visA1Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, visA1Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(visA1), visA1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &visA2Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, visA2Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(visA2), visA2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        std::cout << "Visualisation A initalised" << std::endl;
}


