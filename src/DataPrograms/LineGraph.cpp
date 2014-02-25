#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gui/DisplayUtil.h"
#include "DataReaders/CSVDataReader.h"
#include "DataReaders/DataReader.h"
#include "SourceReaders/RandomCSVSimpleReader.h"
#include "LineGraph.h"

float LineGraph::xAxis[] = {
	// position
	2.0f, -2.0f, -2.9f, 1.0f,
	2.0f, -2.02f, -2.9f, 1.0f,
	-1.8f, -2.00f, -2.9f, 1.0f,
	-1.8f, -2.02f, -2.9f, 1.0f,
	// color
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
        // Box 1
        
   };

// Y axis divisor
float LineGraph::yAxis[] = {
        // position
	-1.82f, 2.0f, -2.9f, 1.0f,
	-1.82f, -2.0f, -2.9f, 1.0f,
	-1.8f, -2.0f, -2.9f, 1.0f,
	-1.8f, 2.0f, -2.9f, 1.0f,
	// color
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
        };


float LineGraph::points[] = {
	// position
	-1.80f, 0.3f, -2.9f, 1.0f,
	-1.6f, 0.7f, -2.9f, 1.0f,
	-1.40f, 0.3f, -2.9f, 1.0f,
	-1.20f, 0.6f, -2.9f, 1.0f,
        -1.0f, 0.7f, -2.9f, 1.0f,
	-0.80f, 0.2f, -2.9f, 1.0f,
	-0.60f, 0.5f, -2.9f, 1.0f,
        -0.40f,0.2f,  -2.9f, 1.0f,
	-0.20f, 0.3f, -2.9f, 1.0f,
	0.0f, 0.7f, -2.9f, 1.0f,
	0.20f, 0.2f, -2.9f, 1.0f,
	0.40f, 0.5f, -2.9f, 1.0f,
        0.60f, 0.5f, -2.9f, 1.0f,
	0.8f , 0.2f, -2.9f, 1.0f,
        1.0f, 0.7f, -2.9f, 1.0f,
	1.20f, 0.2f, -2.9f, 1.0f,
	1.40f, 0.5f, -2.9f, 1.0f,
        1.60f, -0.9f, -2.9f, 1.0f,
	1.8f , 0.2f, -2.9f, 1.0f,
	2.0f , 0.2f, -2.9f, 1.0f,
	
        // colour
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	};

LineGraph::LineGraph(std::string name) {
    
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
    
    this->reader = new CSVDataReader(new RandomCSVSimpleReader());
    
    this->cycle = 0;
    this->companyPrice= *(new vector<float>(20));
    std::fill(companyPrice.begin(),companyPrice.end(),0.5f);
   
}

void LineGraph::InitBuffers(){
    
    // Axes
    glGenBuffers(1, &yAxisBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, yAxisBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yAxis), yAxis, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &xAxisBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, xAxisBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(xAxis), xAxis, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
	glGenBuffers(1, &pointsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    std::cout << this->name << " buffers initialised." << std::endl;
    
}


LineGraph::~LineGraph(){
    
    delete reader;
    
}

void LineGraph::InitProgram(){
    
    std::vector<GLuint> shaderList;
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, this->vertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, this->fragmentShader));
    this->shaderProgram = CreateProgram(shaderList);
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        
    std::cout << this->name << " shader program created." << std::endl;
}
    
    
void LineGraph::ComputeChange(){
    int x=0;
    for(x;x<19;x++){
    	companyPrice.at(x) = companyPrice.at(x+1);
    	}
    	    
    CSVDataType* stockdata = (CSVDataType*) reader->GetNextData();
    int newPrice = std::stoi(stockdata->GetValue("cost"));
    companyPrice.at(19)=((float)(newPrice-100)*3)/100.0f ;
  
    cycle++;

}
void LineGraph::UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection){
    
  // Give updated modelview/perspective matrices to shader
  glUseProgram(this->shaderProgram);
	GLuint modelViewMatUnif = glGetUniformLocation(this->shaderProgram, "modelViewMat");
	glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
	GLuint projectionMatUnif = glGetUniformLocation(this->shaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
  glUseProgram(0);
    
}


void LineGraph::Render(){ 
                
	//Render Axes
    glUseProgram(shaderProgram); 
    glBindBuffer(GL_ARRAY_BUFFER, yAxisBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
    glBindBuffer(GL_ARRAY_BUFFER, xAxisBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
   glUseProgram(0);
   
   
   if(cycle==0){
    this->ComputeChange();
   }
    for(int i=0;i<20;i++){
    	points[i*4 +1] = companyPrice.at(i);
        
          
    }
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
        
    glUseProgram(shaderProgram); 
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)320);
    glDrawArrays(GL_LINE_STRIP,0,20);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    

    glUseProgram(shaderProgram);
    GLuint visBoxOffsetUnif = glGetUniformLocation(shaderProgram, "offset");
    glUniform4f(visBoxOffsetUnif, 0.0f, 0.0f, 0.0f, 0.0f);
    GLuint visBoxScaleUnif = glGetUniformLocation(shaderProgram, "scale");
    glUniform4f(visBoxScaleUnif, 1.0f, 1.0f, 1.0f, 1.0f);
    GLuint visBoxColourUnif = glGetUniformLocation(shaderProgram, "recolor");
    glUniform4f(visBoxColourUnif, 1.0f, 1.0f, 1.0f, 1.0f); 
    glUseProgram(0);
    cycle=(cycle+1) % 50;
}



void LineGraph::SetPosition(glm::mat4 screenOffset, float rotX, float rotY){
    
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