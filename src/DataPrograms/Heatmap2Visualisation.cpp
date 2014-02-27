#include <iostream>
#include <algorithm>

#include "Heatmap2Visualisation.h"
#include "gui/DisplayUtil.h"

using namespace std;

float Heatmap2Visualisation::cell[] = {
    // position
    -2.0f, 0.0f, -2.9f, 1.0f,
    -1.8f, 0.0f, -2.9f, 1.0f,
    -1.8f, 0.2f, -2.9f, 1.0f,
    -2.0f, 0.2f, -2.9f, 1.0f
};

int Heatmap2Visualisation::UPDATE_CYCLE_CNT = 60;

Heatmap2Visualisation::Heatmap2Visualisation(string name) :
    currentIdx(0),
    currentCycle(1)
{
    this->name = name;
    
    companies.clear();
    companies.push_back("ADS");
    companies.push_back("BEI");
    companies.push_back("NOA3");
    companies.push_back("LXS");
    companies.push_back("VOW");
    
    readers.clear();
    retailStates.clear();
    retailStatesCnt.clear();

    for (int i = 0; i < companies.size(); ++i) {
        MDArchiveDataReader* reader = new MDArchiveDataReader("/home/oculus/trading-desk-project/code/src/DataReaders/2014.01.02/" + companies[i] + ".h5");
        readers.push_back(reader);
        retailStates.push_back(reader->getRetailStates());
        retailStatesCnt.push_back(reader->getRetailStateCnt());
        reader = NULL;
    }

    vertexShader = "#version 330\n"
                    "layout(location = 0) in vec4 position;\n"
                    "out vec4 color;\n"
                    "uniform mat4 projectionMat;\n"
                    "uniform mat4 modelViewMat;\n"
                    "uniform mat4 screenOffsetMat;\n"
                    "uniform mat4 rotationMat;\n"
                    "uniform mat4 offset;\n"
                    "uniform vec4 recolor;\n"
                    "void main(){\n"
                        "gl_Position = projectionMat * modelViewMat * screenOffsetMat * rotationMat * offset * position; \n"
                        "color = recolor;\n"
                    "}\n";

    fragmentShader = "#version 330\n"
                     "smooth in vec4 color;\n"
                     "out vec4 outputColor;\n"
                     "void main(){\n"
                         "outputColor = color;\n"
                     "}\n";
    
    cout << "[" + name + "] construction finished" << endl;
}

Heatmap2Visualisation::~Heatmap2Visualisation() {
    for (int i = 0; i < companies.size(); ++i)
        delete readers[i];
}

void Heatmap2Visualisation::InitBuffers() {
    glGenBuffers(1, &cellBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cellBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cell), cell, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    cout << "[" << name << "] buffers loaded" << endl;
}

void Heatmap2Visualisation::InitProgram() {
    vector<GLuint> shaderList;
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, vertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, fragmentShader));
    shaderProgram = CreateProgram(shaderList);
    for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    
    cout << "[" << name << "] program initialized" << endl;
}

void Heatmap2Visualisation::UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection) {
    glUseProgram(shaderProgram);
        GLuint modelViewMatUnif = glGetUniformLocation(shaderProgram, "modelViewMat");
        glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
        GLuint projectionMatUnif = glGetUniformLocation(shaderProgram, "projectionMat");
        glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
    glUseProgram(0);
}

void Heatmap2Visualisation::SetPosition(glm::mat4 screenOffset, float rotX, float rotY){
    glm::mat4 rotationYMat = glm::rotate(rotY , glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 rotationXMat = glm::rotate(rotationYMat, rotX , glm::vec3(1.0, 0.0, 0.0));
    
    glUseProgram(shaderProgram);
        GLuint rotationUnif = glGetUniformLocation(shaderProgram, "rotationMat");
        glUniformMatrix4fv(rotationUnif, 1, GL_FALSE, glm::value_ptr(rotationXMat));

        GLuint screenOffsetUnif = glGetUniformLocation(shaderProgram, "screenOffsetMat");
        glUniformMatrix4fv(screenOffsetUnif, 1, GL_FALSE, glm::value_ptr(screenOffset));
    glUseProgram(0);
}

void Heatmap2Visualisation::Render() {
    bool last = true;
    glUseProgram(shaderProgram);
        for (int i = 0; i < companies.size(); ++i) {
            RetailState retailState;
            RetailState prevRetailState;
            
            int tmp = currentCycle - 1;
            if (tmp < retailStatesCnt[i]) {
                prevRetailState = retailStates[i][tmp];
            } else {
                prevRetailState = retailStates[i][retailStatesCnt[i] - 1];
            }
            if (currentCycle < retailStatesCnt[i]) {
                retailState = retailStates[i][currentCycle];
                last = false;
            } else {
                retailState = retailStates[i][retailStatesCnt[i] - 1];
            }
            
            for (int j = 0; j < 10; ++j) {                
                GLuint offset = glGetUniformLocation(shaderProgram, "offset");
                glm::mat4 offsetMat = glm::translate(glm::mat4(1.0f), glm::vec3((float)(j * 0.21f) + (j >= 5 ? 0.2f : 0.0f), -(float)(i * 0.21f), 0.0f));
                glUniformMatrix4fv(offset, 1, GL_FALSE, glm::value_ptr(offsetMat));

                GLuint color = glGetUniformLocation(shaderProgram, "recolor");
                glm::vec4 colorVec;
                float currVal = (j < 5 ? getBid(retailState, j) : getAsk(retailState, j - 5));
                float lastVal = (j < 5 ? getBid(prevRetailState, j) : getAsk(prevRetailState, j - 5));
                     
                if (lastVal > currVal) {
                    float diff = (lastVal - currVal) / lastVal * 1000.0f;
                    if (diff > 1.0f) diff = 1.0f;
                    if (diff < 0.1) colorVec = glm::vec4(0.1, 0.1, 0.1, 1.0);
                    else colorVec = glm::vec4(diff, 0.0f, 0.0f, 1.0f);
                } else {
                    float diff = (currVal - lastVal) / currVal * 1000.0f;
                    if (diff > 1.0f) diff = 1.0f;
                    if (diff < 0.1) colorVec = glm::vec4(0.1, 0.1, 0.1, 1.0);
                    else colorVec = glm::vec4(0.0f, diff, 0.0f, 1.0f);
                }
                
                glUniform4fv(color, 1, glm::value_ptr(colorVec));
                
                
                glBindBuffer(GL_ARRAY_BUFFER, cellBuffer);
                glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
                    glDrawArrays(GL_QUADS, 0, 4);
                glDisableVertexAttribArray(0);
            }
        }
    glUseProgram(0);

    if (!last) {
        ++currentIdx;
        if (currentIdx % UPDATE_CYCLE_CNT == 0) {
            ++currentCycle;
        }
    }
}

float Heatmap2Visualisation::getBid(RetailState rs, int idx) {
    if (idx == 0) return rs.bid_1_price;
    else if (idx == 1) return rs.bid_2_price;
    else if (idx == 2) return rs.bid_3_price;
    else if (idx == 3) return rs.bid_4_price;
    else return rs.bid_5_price;
}

float Heatmap2Visualisation::getAsk(RetailState rs, int idx) {
    if (idx == 0) return rs.ask_1_price;
    else if (idx == 1) return rs.ask_2_price;
    else if (idx == 2) return rs.ask_3_price;
    else if (idx == 3) return rs.ask_4_price;
    else return rs.ask_5_price;
}