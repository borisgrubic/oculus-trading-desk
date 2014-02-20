#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BuySellBarVisualisation.h"
#include "gui/DisplayUtil.h"
#include "DataReaders/CSVDataReader.h"
#include "DataReaders/DataReader.h"
#include "SourceReaders/RandomCSVSimpleReader.h"
#include "gui/ScreenModel.h"

int cycle2 = 0;

const std::string BuySellBarVisualisation::vertexShader = 
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

const std::string BuySellBarVisualisation::fragmentShader =
"#version 330\n"
"smooth in vec4 theColor;\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"   outputColor = theColor;\n"
	"}\n";

// Divisor
const float BuySellBarVisualisation::divisor[] = {
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

// A single bar to be duplicated and resized
const float BuySellBarVisualisation::bar[] = {
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

BuySellBarVisualisation::BuySellBarVisualisation(std::string name){
    
    this->name = name;
    
    this->reader = new CSVDataReader(new RandomCSVSimpleReader());
    
    this->companyOffsets = *(new vector<float>(5));
    this->companyScales = *(new vector<float>(5));
    this->companyColours = *(new vector<glm::vec3>(5));
        std::fill(companyOffsets.begin(),companyOffsets.end(),-0.01f);
        std::fill(companyScales.begin(),companyScales.end(),0.0f);
        std::fill(companyColours.begin(),companyColours.end(), *(new glm::vec3(1.0f, 0.0f, 0.0f)));
    
}

BuySellBarVisualisation::~BuySellBarVisualisation(){
    
    delete reader;
    delete &companyOffsets;
    delete &companyScales;
    delete &companyColours;
    
}

void BuySellBarVisualisation::InitBuffers(){
    
    // Divisor
    glGenBuffers(1, &divisorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, divisorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(divisor), divisor, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Bar
    glGenBuffers(1, &barBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, barBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bar), bar, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    std::cout << this->name << " buffers initialised." << std::endl;
    
}

void BuySellBarVisualisation::InitProgram(){
    
    std::vector<GLuint> shaderList;
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, vertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, fragmentShader));
    shaderProgram = CreateProgram(shaderList);
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        
    std::cout << this->name << " shader program created." << std::endl;
    
}

void BuySellBarVisualisation::ComputeChange(){
    
    CSVDataType* stockdata = (CSVDataType*) reader->GetNextData();
    
    // Compute price * volume bought or sold
    int totalcost = std::stoi(stockdata->GetValue("cost")) * std::stoi(stockdata->GetValue("volume"));
    if(totalcost>1000) totalcost = 1000;
        
    float scalex = ((float)totalcost/1000);

    // std::cout << scalex << std::endl;

    std::string type = stockdata->GetValue("type");
    std::string company = stockdata->GetValue("company");

    // Calculate offset and colour
    float boxr = 1.0f;
    float boxg = 0.0f;
    float boxb = 0.0f;
    if(type=="buy"){
            boxr = 0.8f;
            boxg = 0.0f;
            boxb = 0.2f;
            scalex = scalex * 1.0f;
    } else {
            boxr = 0.0f;
            boxg = 0.8f;
            boxb = 0.4f;    
            scalex = scalex * -1.0f;
    }
        
        
        // Further evidence of how horrendous this code is
        if(company=="GOOG"){
            companyScales.at(0) = companyScales.at(0) + scalex;
            if(companyScales.at(0) > 1.5f) companyScales.at(0) = 1.5f;
            if(companyScales.at(0) < -1.5f) companyScales.at(0) = -1.5f;
            companyColours.at(0).x = boxr;
            companyColours.at(0).y = boxg;
            companyColours.at(0).z = boxb;
        } else if(company=="FB"){
            companyScales.at(1) = companyScales.at(1) + scalex;
            if(companyScales.at(1) > 1.5f) companyScales.at(1) = 1.5f;
            if(companyScales.at(1) < -1.5f) companyScales.at(1) = -1.5f;
            companyColours.at(1).x = boxr;
            companyColours.at(1).y = boxg;
            companyColours.at(1).z = boxb;
        } else if(company=="AAPL"){
            companyScales.at(2) = companyScales.at(2) + scalex;
            if(companyScales.at(2) > 1.5f) companyScales.at(2) = 1.5f;
            if(companyScales.at(2) < -1.5f) companyScales.at(2) = -1.5f;
            companyColours.at(2).x = boxr;
            companyColours.at(2).y = boxg;
            companyColours.at(2).z = boxb;
        } else if(company=="MSFT"){
            companyScales.at(3) = companyScales.at(3) + scalex;
            if(companyScales.at(3) > 1.5f) companyScales.at(3) = 1.5f;
            if(companyScales.at(3) < -1.5f) companyScales.at(3) = -1.5f;
            companyColours.at(3).x = boxr;
            companyColours.at(3).y = boxg;
            companyColours.at(4).z = boxb;
        } else if(company=="AMZN"){
            companyScales.at(4) = companyScales.at(4) + scalex;
            if(companyScales.at(4) > 1.5f) companyScales.at(4) = 1.5f;
            if(companyScales.at(4) < -1.5f) companyScales.at(4) = -1.5f;
            companyColours.at(4).x = boxr;
            companyColours.at(4).y = boxg;
            companyColours.at(4).z = boxb;
        }
    
    
}

void BuySellBarVisualisation::UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection){
    
  // Give updated modelview/perspective matrices to shader
  glUseProgram(shaderProgram);
	GLuint modelViewMatUnif = glGetUniformLocation(shaderProgram, "modelViewMat");
	glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
	GLuint projectionMatUnif = glGetUniformLocation(shaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
  glUseProgram(0);
    
}

void BuySellBarVisualisation::Render(){ 
  
    if(cycle2==0){
        ComputeChange();
    }
    
    // Render divisor
    glUseProgram(shaderProgram); 
    glBindBuffer(GL_ARRAY_BUFFER, divisorBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    
   // Render each box
   for(int i=0; i< 14; i++){
              
       // Set offset, scale 
       GLuint visBoxScaleUnif = glGetUniformLocation(shaderProgram, "scale");
       glUniform4f(visBoxScaleUnif, companyScales.at(i%5), 1.0f, 1.0f, 1.0f);
       
       float boxoff = 0.0f;
       if(companyScales.at(i%5)<0){
           // green
           companyColours.at(i%5).x = 0.0f;
           companyColours.at(i%5).y = 0.8f;
           //companyColours.at(i%5).z = 0.0f;
           //offset to left
           boxoff = -0.01f;
       } else {
           // red
           companyColours.at(i%5).x = 0.8f;
           companyColours.at(i%5).y = 0.0f;
           //companyColours.at(i%5).z = 0.0f;
           //offset to right
           boxoff = 0.01f;
       }

       GLuint visBoxOffsetUnif = glGetUniformLocation(shaderProgram, "offset");
       glUniform4f(visBoxOffsetUnif, boxoff, (float)-0.2*i, 0.0f, 0.0f);
       
       GLuint visBoxColourUnif = glGetUniformLocation(shaderProgram, "recolor");
       glUniform4f(visBoxColourUnif, companyColours.at(i%5).x, companyColours.at(i%5).y, companyColours.at(i%5).z, 1.0f);
        
       // Render quads
        glBindBuffer(GL_ARRAY_BUFFER, barBuffer);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)64);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);  
       
       
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
         
    
    cycle2 = (cycle2 +1) %50;
}
