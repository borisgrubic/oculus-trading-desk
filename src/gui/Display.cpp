#include <cstdlib>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>

#include <GL/glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "OVR.h"
#include "Display.h"
#include "ScreenModel.h"
#include "DisplayUtil.h"
#include "io/Device.h"

#include "DataReaders/CSVDataReader.h"
#include "DataTypes/CSVDataType.h"
#include "SourceReaders/RandomCSVSimpleReader.h"
#include "DataPrograms/Visualisation.h"
#include "DataPrograms/BuySellBarVisualisation.h"

GLFWwindow* window;

Device* device;

glm::ivec2 windowPosition;

glm::mat4 projection;
glm::mat4 modelview;
glm::mat4 modelviewProjection;

using namespace OVR;

Visualisation* visB;

/* Global variables to hold device handles*/
float aspectRatio;
float fov;

void Init(bool displayOnScreen){
    
	// Get the OVR Device
	device = new Device();

	// Initialise GLFW 
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(0);
	}

	glfwWindowHint(GLFW_DEPTH_BITS, 16);

	windowPosition = glm::ivec2(device->Info.DesktopX, device->Info.DesktopY);

	// Get monitor at Info.DesktopX/Y (should be OVR)
	GLFWmonitor* hmdMonitor = getMonitorAtPosition(windowPosition);

	// If display on screen is enabled, window will render on main monitor,
	// otherwise it will display on the rift
	if (!displayOnScreen){

		if (!hmdMonitor){
			std::cout << "Unable to find position of OVR display" << std::endl;
		} 

	} else {

                std::cout << "Display on screen is enabled" << std::endl;
            
		// Get primary monitor
		int px, py;
		GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
		glfwGetMonitorPos(primaryMonitor, &px, &py);

		windowPosition = glm::ivec2(px+200, py+200);

	}

	// Create the window with OVR resolution
	window = glfwCreateWindow(device->Info.HResolution, device->Info.VResolution, "Virtual Reality Trading Desk", NULL, NULL);

	// Check window was created
	if (!window){
		std::cout << "Failed to create window" << std::endl;
		exit(0);
	}

	// Set the window position 
	glfwSetWindowPos(window, windowPosition.x, windowPosition.y);

	// Set openGL context to the newly created window
	glfwMakeContextCurrent(window);

	// Initialise GLEW
	if (glewInit() != GLEW_OK){
		std::cout << "Failed to initialise GLEW" << std::endl;
		exit(0);
	}

	// Initialise the shader program
	InitProgram();

        InitBuffers();
        InitTestGraphBuffers();
        InitVisualisationBuffers();
        
        visB = new BuySellBarVisualisation("Stock Buy Sell Data");
        visB->InitBuffers();
        visB->InitProgram();
        
}

void UpdateView(glm::vec3 eyeProjectionOffset, glm::vec3 eyeModelviewOffset){

	// Fetch the pitch, roll and yaw
	glm::vec3 eulerAngles;
	device->pFusionResult->GetOrientation().GetEulerAngles<Axis_X, Axis_Y, Axis_Z, Rotate_CW, Handed_R>(&eulerAngles.x, &eulerAngles.y, &eulerAngles.z);

        // Check we are inside limits of scope of view
        // and if not set view to the closest limit
        if(eulerAngles.x>1.0){
            eulerAngles.x = 1.0;
        } else if(eulerAngles.x<-1.0){
            eulerAngles.x = -1.0;
        }
        if(eulerAngles.y>1.3){
            eulerAngles.y = 1.3;
        } else if(eulerAngles.y<-1.3){
            eulerAngles.y = -1.3;
        }
        
        // Convert to GLM quaternion
        glm::quat orientation = glm::quat(eulerAngles);
        
	// Translate the modelview by the quaternion orientation
	modelview =  glm::mat4_cast(orientation) * (device->modelview);

        
        glm::mat4 eyeProjection = glm::translate(device->projection, eyeProjectionOffset);
        glm::mat4 eyeModelview = glm::translate(glm::mat4(), eyeModelviewOffset) * modelview;
        
	// Give the updated view matrices to all shaders
	glUseProgram(screenShaderProgram);
	GLuint modelViewMatUnif = glGetUniformLocation(screenShaderProgram, "modelViewMat");
	glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
	GLuint projectionMatUnif = glGetUniformLocation(screenShaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
	glUseProgram(0);
        
        visB->UpdateView(eyeModelview, eyeProjection);

}

void RenderScreens(){

	glUseProgram(screenShaderProgram);
	// Main screen
	glBindBuffer(GL_ARRAY_BUFFER, mainScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Top screen
	glBindBuffer(GL_ARRAY_BUFFER, topScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Bottom screen
	glBindBuffer(GL_ARRAY_BUFFER, bottomScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Left screen
	glBindBuffer(GL_ARRAY_BUFFER, leftScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Right screen
	glBindBuffer(GL_ARRAY_BUFFER, rightScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
        glUseProgram(0);   

}

// For GL debugging
void glError(){
    
    GLenum errCode;
    const GLubyte *errString;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        fprintf (stdout, "OpenGL Error: %s\n", errString);
    }  
    
}

void RunDisplay(){
	while (!glfwWindowShouldClose(window)){

		// Set the viewport to the oculus rift size and clear buffers
		glViewport(0, 0, device->Info.HResolution, device->Info.VResolution);
		glClearColor(0.1f, 1.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// We need to render twice, once for each eye
		for (int i = 0; i < 2; ++i) {
			
                        glm::vec3 eyeProjectionOffset(-(device->stereoConfig.GetProjectionCenterOffset()), 0, 0);
                        glm::vec3 eyeModelviewOffset = glm::vec3(-(device->stereoConfig.GetIPD()), 0, 0);
                                            
                        glm::vec3 test = glm::vec3(-0.5f, 0.0f, 0.0f);
                        Device::Eye eye = device->eyes[i];
                        
			// Get the eye
			if (i==0) {
                                eyeModelviewOffset *= -1;
                                eyeProjectionOffset *= -1;
                                test *= -1;
                        }

			// Size the viewport for the current eye
			glViewport(eye.viewportLocation.x, 0, device->Info.HResolution / 2, device->Info.VResolution);
                        
			// Get sensor information and render updated view
			UpdateView(eyeProjectionOffset, eyeModelviewOffset);

			// Render screen quads
			RenderScreens();
                        glError();
                        visB->Render();
                        glError();
                        

		}
                                
                
		glfwSwapBuffers(window);
		glfwPollEvents();
                
	}
}

void clearWindowDisplay(){

	// Destroy the window
	if (window){
		glfwDestroyWindow(window);
	}

}

