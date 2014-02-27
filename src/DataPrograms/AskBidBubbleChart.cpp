#include <iostream>
#include <string>
#include <math.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AskBidBubbleChart.h"
#include "gui/DisplayUtil.h"

// X axis divisor
float AskBidBubbleChart::xaxis[] = {
	// position
	2.0f, 0.01f, -2.9f, 1.0f,
	2.0f, -0.01f, -2.9f, 1.0f,
	-2.0f, -0.01f, -2.9f, 1.0f,
	-2.0f, 0.01f, -2.9f, 1.0f,
	// color
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
        // Box 1
        
   };

// Y axis divisor
float AskBidBubbleChart::yaxis[] = {
        // position
	0.01f, 2.0f, -2.9f, 1.0f,
	0.01f, -2.0f, -2.9f, 1.0f,
	-0.01f, -2.0f, -2.9f, 1.0f,
	-0.01f, 2.0f, -2.9f, 1.0f,
	// color
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
        };

// Y axis divisor
float AskBidBubbleChart::bubble[] = {
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

AskBidBubbleChart::AskBidBubbleChart(std::string name) {
    
    this->name = name;
    
    this->vertexShader = "#version 330\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "smooth out vec4 theColor;\n"   
        "uniform mat4 projectionMat;\n"
        "uniform mat4 modelViewMat;\n"
        "uniform mat4 screenOffsetMat;\n"
        "uniform mat4 rotationMat;\n"
        "uniform vec4 offset;\n"
        "uniform vec4 scale;\n"
        "uniform vec4 recolor;\n"
        "void main(){\n"
        "gl_Position = projectionMat * modelViewMat * (screenOffsetMat * rotationMat *((position*scale) + offset)); \n"
        "theColor = recolor;\n"
	"}\n";
    
    this-> fragmentShader = "#version 330\n"
        "smooth in vec4 theColor;\n"
        "out vec4 outputColor;\n"
        "void main()\n"
        "{\n"
        "   outputColor = theColor;\n"
                "}\n";
            
    this->cycle = 0;
    this->cycle2 = 0;
    
    this-> numCompanies = 0;
    
    // Initialise company readers
    this->readers = *(new std::vector<MDArchiveDataReader*>());
    this->retailStateCounts = *(new std::vector<int>());
    
    // Set companies
    std::vector<std::string> comps;
    comps.push_back("ADS");
    comps.push_back("BEI");
    comps.push_back("NOA3");
    setCompanies(comps);
    
    // Initialise ask/bid prices/volumes
    this->askVolumes = *(new std::vector<float>(numCompanies));
    std::fill(askVolumes.begin(),askVolumes.end(),0.0f);
    this->bidVolumes = *(new std::vector<float>(numCompanies));
    std::fill(bidVolumes.begin(),bidVolumes.end(),0.0f);
    this->askPrices = *(new std::vector<float>(numCompanies));
    std::fill(askPrices.begin(),askPrices.end(),0.0f);
    this->bidPrices = *(new std::vector<float>(numCompanies));
    std::fill(bidPrices.begin(),bidPrices.end(),0.0f);
    
    
    

    
}

AskBidBubbleChart::~AskBidBubbleChart(){
    
    delete &askVolumes;
    delete &bidVolumes;
    delete &askPrices;
    delete &bidPrices;
    delete &readers;
    delete &retailStateCounts;
    
}

void AskBidBubbleChart::InitBuffers(){
    
    glGenBuffers(1, &xaxisBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, xaxisBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(xaxis), xaxis, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &yaxisBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, yaxisBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yaxis), yaxis, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &bubbleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bubbleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bubble), bubble, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    std::cout << this->name << " buffers initialised." << std::endl;
    
}

void AskBidBubbleChart::InitProgram(){
    
    std::vector<GLuint> shaderList;
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, this->vertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, this->fragmentShader));
    this->shaderProgram = CreateProgram(shaderList);
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        
    std::cout << this->name << " shader program created." << std::endl;
    
}

void AskBidBubbleChart::ComputeChange(){

    int c = 0;
    for(std::vector<MDArchiveDataReader*>::iterator it = readers.begin(); it != readers.end(); ++it) {
        
        RetailState* rs = (*it)->getRetailStates();
        
        int index = 0;
        if(retailStateCounts.at(c)<= cycle){
            index = retailStateCounts.at(c);
        } else {
            index = cycle;
        }
        
        
        // Position based on bid and ask price
        bidPrices.at(c) = (float)rs[index].bid_1_price * 0.01;
        askPrices.at(c) = (float)rs[index].ask_1_price * 0.01;
        
        // Size based on bid and ask volume
        askVolumes.at(c) = log(-1.0f*(float)rs[index].ask_1_volume) * 0.06f;
        bidVolumes.at(c) = log((float)rs[index].bid_1_volume) * 0.06f; 
                
        c++;
    }
    
}

void AskBidBubbleChart::UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection){
    
  // Give updated modelview/perspective matrices to shader
  glUseProgram(this->shaderProgram);
	GLuint modelViewMatUnif = glGetUniformLocation(this->shaderProgram, "modelViewMat");
	glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
	GLuint projectionMatUnif = glGetUniformLocation(this->shaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
  glUseProgram(0);
    
}

void AskBidBubbleChart::Render(){ 
  
   if(cycle2==0){
        this->ComputeChange();
   }
    
    // Render x and y axis
    glUseProgram(shaderProgram); 
    glBindBuffer(GL_ARRAY_BUFFER, xaxisBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, yaxisBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glUseProgram(0);
    
    for(int i=0; i<numCompanies; i++){
        drawCircle(bidVolumes.at(i), bidPrices.at(i), askPrices.at(i), 50, glm::vec3(1.8f, 0.1f, 0.1f));
        drawCircle(askVolumes.at(i), bidPrices.at(i), askPrices.at(i), 50, glm::vec3(0.8f, 0.8f, 0.1f));
    }
        
    // Reset offset and scale
    glUseProgram(shaderProgram);
    GLuint visBoxOffsetUnif = glGetUniformLocation(shaderProgram, "offset");
    glUniform4f(visBoxOffsetUnif, 0.0f, 0.0f, 0.0f, 0.0f);
    GLuint visBoxScaleUnif = glGetUniformLocation(shaderProgram, "scale");
    glUniform4f(visBoxScaleUnif, 1.0f, 1.0f, 1.0f, 1.0f);
    GLuint visBoxColourUnif = glGetUniformLocation(shaderProgram, "recolor");
    glUniform4f(visBoxColourUnif, 1.0f, 1.0f, 1.0f, 1.0f); 
    glUseProgram(0);
         
    
    cycle = cycle +1;
    cycle2 = (cycle2 +1) % 20;
}

void AskBidBubbleChart::SetPosition(glm::mat4 screenOffset, float rotX, float rotY){
    
    glm::mat4 rotationYMat = glm::rotate(rotY , glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 rotationXMat = glm::rotate(rotationYMat, rotX , glm::vec3(1.0, 0.0, 0.0));
    
    glUseProgram(shaderProgram);
        GLuint rotationUnif = glGetUniformLocation(shaderProgram, "rotationMat");
	glUniformMatrix4fv(rotationUnif, 1, GL_FALSE, glm::value_ptr(rotationXMat));
     glUseProgram(0);
    
     glUseProgram(shaderProgram);
        GLuint screenOffsetUnif = glGetUniformLocation(shaderProgram, "screenOffsetMat");
	glUniformMatrix4fv(screenOffsetUnif, 1, GL_FALSE, glm::value_ptr(screenOffset));
     glUseProgram(0);
    
}

void AskBidBubbleChart::setCompanies(std::vector<std::string> companies){
        
    for(std::vector<std::string>::iterator it = companies.begin(); it != companies.end(); ++it) {
       
        MDArchiveDataReader* reader = new MDArchiveDataReader("/home/oculus/trading-desk-project/code/src/DataReaders/2014.01.02/" + *it + ".h5");
        readers.push_back(reader);
        retailStateCounts.push_back(reader->getRetailStateCnt());
        
        numCompanies++;
    }
    
}

void AskBidBubbleChart::drawCircle(float r, float cx, float cy, int segments, glm::vec3 colour){
    
    
    
    glUseProgram(shaderProgram); 
    GLuint visBoxOffsetUnif = glGetUniformLocation(shaderProgram, "offset");
    glUniform4f(visBoxOffsetUnif, cx, cy, -2.9f, 0.0f);
    GLuint visBoxColourUnif = glGetUniformLocation(shaderProgram, "recolor");
    glUniform4f(visBoxColourUnif, colour.x, colour.y, colour.z, 1.0f); 
    glBegin(GL_TRIANGLE_FAN); 
	for(int i = 0; i < segments; i++){ 
            float theta = 2.0f * 3.1415926f * float(i) / float(segments);//get the current angle 

            float x = r * cosf(theta);//calculate the x component 
            float y = r * sinf(theta);//calculate the y component 

            glVertex2f(x + 0, y + 0);//output vertex 
    	} 
    glEnd(); 
    glUniform4f(visBoxColourUnif, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(visBoxOffsetUnif, 0.0f, 0.0f, 0.0f, 0.0f);
    glUseProgram(0); 
    
}