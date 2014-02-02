#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "OVR.h"
#include "Display.h"
#include "ScreenModel.h"
#include "DisplayUtil.h"
#include "Device.h"

#include <GLFW/glfw3.h>

GLFWwindow* window;

Device* device;

glm::ivec2 windowPosition;

glm::mat4 projection;
glm::mat4 modelview;
glm::mat4 modelviewProjection;

using namespace OVR;

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

}

void UpdateView(glm::mat4 eyeModelView, glm::mat4 eyeProjection){

	// Fetch the pitch, roll and yaw
	glm::vec3 eulerAngles;
	device->pFusionResult->GetOrientation().GetEulerAngles<Axis_X, Axis_Y, Axis_Z, Rotate_CW, Handed_R>(&eulerAngles.x, &eulerAngles.y, &eulerAngles.z);

	// Convert to GLM quaternion
	glm::quat orientation = glm::quat(eulerAngles);

	// Translate the modelview by the quaternion orientation
	modelview = eyeModelView * glm::mat4_cast(orientation) * (device->modelview);

	// Give the updated view matrices to the shader
	glUseProgram(shaderProgram);
	GLuint modelViewMatUnif = glGetUniformLocation(shaderProgram, "modelViewMat");
	glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelview));
	GLuint projectionMatUnif = glGetUniformLocation(shaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
	glUseProgram(0);

}

void RenderScreens(){

	glUseProgram(shaderProgram);
	// Main screen
	glBindBuffer(GL_ARRAY_BUFFER, mainScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)60);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Top screen
	glBindBuffer(GL_ARRAY_BUFFER, topScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)60);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Bottom screen
	glBindBuffer(GL_ARRAY_BUFFER, bottomScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)60);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Left screen
	glBindBuffer(GL_ARRAY_BUFFER, leftScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)60);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// Right screen
	glBindBuffer(GL_ARRAY_BUFFER, rightScreenBuffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)60);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0); 

}

void RunDisplay(){
	while (!glfwWindowShouldClose(window)){

		// Set the viewport to the oculus rift size and clear buffers
		glViewport(0, 0, device->Info.HResolution, device->Info.VResolution);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// We need to render twice, once for each eye
		for (int i = 0; i < 2; ++i) {
			
			// Get the eye
			Device::Eye eye = device->eyes[i];

			// Size the viewport for the current eye
			glViewport(eye.viewportLocation.x, 0, device->Info.HResolution / 2, device->Info.VResolution);

			// Get sensor information and render updated view
			UpdateView(eye.modelviewOffset, eye.projectionOffset);

			// Render screen quads
			RenderScreens();

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

