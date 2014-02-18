#include <string>
#include <GL/glew.h>
#include "BuySellBarVisualisation.h"
#include "DisplayUtil.h"

const std::string name = "Buy Sell Data Bar Graph";

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
    
    // Do something like define colours here
    
    
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
    
}

void BuySellBarVisualisation::CreateProgram(){

    std::vector<GLuint> shaderList;
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, vertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, fragmentShader));
    visAShaderProgram = CreateProgram(shaderList);
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        
    std::cout << this->name << " shader program created." << std::endl;
}

void BuySellBarVisualisation::ComputeChange(){
    
    CSVDataType* stockdata = (CSVDataType*) reader->GetNextData();
    
    // Perform computations on data here
    
}

void BuySellBarVisualisation::Render(){
    
  // Render to GL
    
}
