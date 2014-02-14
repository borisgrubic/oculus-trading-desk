#include <cstdlib>
#include <iostream>
#include <boost/algorithm/string.hpp>

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

#include "SourceReaders/RandomCSVSimpleReader.h"



GLFWwindow* window;

Device* device;

glm::ivec2 windowPosition;

glm::mat4 projection;
glm::mat4 modelview;
glm::mat4 modelviewProjection;

RandomCSVSimpleReader* reader;

vector<float> companyOffsets;
vector<float> companyScales;
vector<glm::vec3> companyColours;

int cycle;

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
        
        // Create a simple random reader
        reader = new RandomCSVSimpleReader();
        
        // Initialise company values
        companyOffsets = *(new vector<float>(5));
        companyScales = *(new vector<float>(5));
        companyColours = *(new vector<glm::vec3>(5));
        std::fill(companyOffsets.begin(),companyOffsets.end(),-0.01f);
        std::fill(companyScales.begin(),companyScales.end(),-0.5f);
        std::fill(companyColours.begin(),companyColours.end(), *(new glm::vec3(1.0f, 0.0f, 0.0f)));
        
        // Set the default offset and scale for visualistion A
        GLuint visBoxOffsetUnif = glGetUniformLocation(visAShaderProgram, "offset");
        glUniform4f(visBoxOffsetUnif, -0.01f, 0.0f, 0.0f, 0.0f);
        GLuint visBoxScaleUnif = glGetUniformLocation(visAShaderProgram, "scale");
        glUniform4f(visBoxScaleUnif, -0.5f, 1.0f, 1.0f, 1.0f);
        GLuint visBoxColourUnif = glGetUniformLocation(visAShaderProgram, "recolor");
        glUniform4f(visBoxColourUnif, 1.0f, 1.0f, 1.0f, 1.0f); 
        
        cycle = 0;
        
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
        glUseProgram(visAShaderProgram);
	modelViewMatUnif = glGetUniformLocation(visAShaderProgram, "modelViewMat");
	glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
	projectionMatUnif = glGetUniformLocation(visAShaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
	glUseProgram(0);

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

// Really really horrible code
// TODO: Move this all into its own data program class and do the same for all visualisations
void RenderVisualisationA(){
    
    
    if(cycle==0){
        // Get the next data change from the reader
        string stockchange = reader->ReadNextData();
        std::vector<std::string> stockdata;
        boost::split(stockdata, stockchange, boost::is_any_of(","));

        std::cout << stockchange << std::endl;

        // Compute price * volume bought or sold
        int totalcost = std::stoi(stockdata.at(2)) * std::stoi(stockdata.at(3));
        if(totalcost>1000) totalcost = 1000;
        
        float scalex = ((float)totalcost/1000);

        std::cout << scalex << std::endl;

        // Render the boxes with appropriate offset and scale
        //Divisor

        std::string type = stockdata.at(0);
        std::string company = stockdata.at(1);

        // Calculate offset
        //float boxoff = 0.0f;
        float boxscale = 1.0f;
        float boxr = 1.0f;
        float boxg = 0.0f;
        float boxb = 0.0f;
        if(type=="buy"){
            //boxoff = 0.01f;
            boxr = 0.8f;
            boxg = 0.0f;
            boxb = 0.0f;
            //boxscale = 1.0f * scalex;
            scalex = scalex * 1.0f;
        } else {
            //boxoff = -0.01f;
            boxr = 0.0f;
            boxg = 0.8f;
            boxb = 0.0f;    
           // boxscale = -1.0f * scalex;
            scalex = scalex * -1.0f;
        }
        
        std::cout << "boxscale " << boxscale << std::endl;
        
        // Further evidence of how horrendous this code is
        if(company=="GOOG"){
            //companyOffsets.at(0) = boxoff;
            //companyScales.at(0) = boxscale;
            companyScales.at(0) = companyScales.at(0) + scalex;
            if(companyScales.at(0) > 1.5f) companyScales.at(0) = 1.5f;
            if(companyScales.at(0) < -1.5f) companyScales.at(0) = -1.5f;
            companyColours.at(0).x = boxr;
            companyColours.at(0).y = boxg;
            companyColours.at(0).z = boxb;
        } else if(company=="FB"){
            //companyOffsets.at(1) = boxoff;
            //companyScales.at(1) = boxscale;
            companyScales.at(1) = companyScales.at(1) + scalex;
            if(companyScales.at(1) > 1.5f) companyScales.at(1) = 1.5f;
            if(companyScales.at(1) < -1.5f) companyScales.at(1) = -1.5f;
            companyColours.at(1).x = boxr;
            companyColours.at(1).y = boxg;
            companyColours.at(1).z = boxb;
        } else if(company=="AAPL"){
            //companyOffsets.at(2) = boxoff;
            //companyScales.at(2) = boxscale;
            companyScales.at(2) = companyScales.at(2) + scalex;
            if(companyScales.at(2) > 1.5f) companyScales.at(2) = 1.5f;
            if(companyScales.at(2) < -1.5f) companyScales.at(2) = -1.5f;
            companyColours.at(2).x = boxr;
            companyColours.at(2).y = boxg;
            companyColours.at(2).z = boxb;
        } else if(company=="MSFT"){
            //companyOffsets.at(3) = boxoff;
            //companyScales.at(3) = boxscale;
            companyScales.at(3) = companyScales.at(3) + scalex;
            if(companyScales.at(3) > 1.5f) companyScales.at(3) = 1.5f;
            if(companyScales.at(3) < -1.5f) companyScales.at(3) = -1.5f;
            companyColours.at(3).x = boxr;
            companyColours.at(3).y = boxg;
            companyColours.at(4).z = boxb;
        } else if(company=="AMZN"){
            //companyOffsets.at(4) = boxoff;
            //companyScales.at(4) = boxscale;
            companyScales.at(4) = companyScales.at(4) + scalex;
            if(companyScales.at(4) > 1.5f) companyScales.at(4) = 1.5f;
            if(companyScales.at(4) < -1.5f) companyScales.at(4) = -1.5f;
            companyColours.at(4).x = boxr;
            companyColours.at(4).y = boxg;
            companyColours.at(4).z = boxb;
        }
    }
    
    glUseProgram(visAShaderProgram); 
    glBindBuffer(GL_ARRAY_BUFFER, visA1Buffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    
    // Render each box
   for(int i=0; i< 5; i++){
       
       // Set offset, scale 

       GLuint visBoxScaleUnif = glGetUniformLocation(visAShaderProgram, "scale");
       glUniform4f(visBoxScaleUnif, companyScales.at(i%5), 1.0f, 1.0f, 1.0f);
       
       float boxoff = 0.0f;
       if(companyScales.at(i%5)<0){
           // green
           companyColours.at(i%5).x = 0.0f;
           companyColours.at(i%5).y = 0.8f;
           companyColours.at(i%5).z = 0.0f;
           //offset to left
           boxoff = -0.01f;
       } else {
           // red
           companyColours.at(i%5).x = 0.8f;
           companyColours.at(i%5).y = 0.0f;
           companyColours.at(i%5).z = 0.0f;
           //offset to right
           boxoff = 0.01f;
       }

       GLuint visBoxOffsetUnif = glGetUniformLocation(visAShaderProgram, "offset");
       glUniform4f(visBoxOffsetUnif, boxoff, (float)-0.2*i, 0.0f, 0.0f);
       
       GLuint visBoxColourUnif = glGetUniformLocation(visAShaderProgram, "recolor");
       glUniform4f(visBoxColourUnif, companyColours.at(i%5).x, companyColours.at(i%5).y, companyColours.at(i%5).z, 1.0f);
        
       std::cout << "r" << companyColours.at(i%5).x << " g " << companyColours.at(i%5).y << " b " << companyColours.at(i%5).z << std::endl;
       std::cout << "scale:"  << companyScales.at(i%5) << std::endl;
        // Render quads
        glBindBuffer(GL_ARRAY_BUFFER, visA2Buffer);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);  
        
    }
    
    // Reset offset and scale
     GLuint visBoxOffsetUnif = glGetUniformLocation(visAShaderProgram, "offset");
     glUniform4f(visBoxOffsetUnif, 0.0f, 0.0f, 0.0f, 0.0f);
     GLuint visBoxScaleUnif = glGetUniformLocation(visAShaderProgram, "scale");
     glUniform4f(visBoxScaleUnif, 1.0f, 1.0f, 1.0f, 1.0f);
     GLuint visBoxColourUnif = glGetUniformLocation(visAShaderProgram, "recolor");
       glUniform4f(visBoxColourUnif, 1.0f, 1.0f, 1.0f, 1.0f); 
      

    glUseProgram(0);
    
    cycle = (cycle +1) % 40;
        
    
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
                        RenderVisualisationA();

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

