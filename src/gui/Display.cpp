#include <cstdlib>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>

#include <GL/glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
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
#include "DataPrograms/AskBidBubbleChart.h"

GLFWwindow* window;

Device* device;

glm::ivec2 windowPosition;

glm::mat4 projection;
glm::mat4 modelview;
glm::mat4 modelviewProjection;

using namespace OVR;

Visualisation* visB;
Visualisation* visA;
Visualisation* visC;
Visualisation* visD;
Visualisation* visE;

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
        glfwWindowHint(GLFW_SAMPLES, 8);

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
        
        // Enable antialiasing
        glEnable(GL_BLEND);
        glEnable(GL_POLYGON_SMOOTH);
        //glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Initialise the shader program
	InitProgram();
        InitBuffers();
        
        // Create visualisations
        // Visualisation A (main screen)
        visA = new BuySellBarVisualisation("Stock Buy Sell Data 1");
        visA->InitBuffers();
        visA->InitProgram();
        
        // Visualisation B (right screen) 
        visB = new AskBidBubbleChart("Ask bid bubble chart 1");
        visB->InitBuffers();
        visB->InitProgram();
        // Visualisation C (left screen)
        visC = new AskBidBubbleChart("Ask bid bubble chart 2");
        visC->InitBuffers();
        visC->InitProgram();
        // Visualisation D (top screen)
        visD = new BuySellBarVisualisation("Stock Buy Sell Data 2");
        visD->InitBuffers();
        visD->InitProgram();
        // Visualisation E (bottom screen)
        visE = new BuySellBarVisualisation("Stock Buy Sell Data 3");
        visE->InitBuffers();
        visE->InitProgram();
        
        // Set visualisation positions
        visA->SetPosition(glm::translate(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
        visB->SetPosition(glm::translate(2.0f, 0.0f, 0.0f), 0.0f, -70.0f);
        visC->SetPosition(glm::translate(-2.0f, 0.0f, 0.0f), 0.0f, 70.0f);
        visD->SetPosition(glm::translate(0.0f, 2.0f, 0.0f), 45.0f, 0.0f);
        visE->SetPosition(glm::translate(0.0f, -2.0f, 0.0f), -45.0f, 0.0f);
        
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
        if(eulerAngles.y>1.4){
            eulerAngles.y = 1.4;
        } else if(eulerAngles.y<-1.4){
            eulerAngles.y = -1.4;
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
        
        visA->UpdateView(eyeModelview, eyeProjection);
        visB->UpdateView(eyeModelview, eyeProjection);
        visC->UpdateView(eyeModelview, eyeProjection);
        visD->UpdateView(eyeModelview, eyeProjection);
        visE->UpdateView(eyeModelview, eyeProjection);

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
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
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
                        visA->Render();
                        visB->Render();
                        visC->Render();
                        visD->Render();
                        visE->Render();
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

