#include <iostream>
#include <algorithm>

#include "Graph3D.h"
#include "gui/DisplayUtil.h"

using namespace std;

float Graph3D::block[] = {
    // positions
    -0.2f, -0.015f, 0.1f, 1.0f,
     0.2f, -0.015f, 0.1f, 1.0f,
     0.2f, -0.015f, -0.1f, 1.0f,
    -0.2f, -0.015f, -0.1f, 1.0f,
    
    -0.2f, 0.015f, 0.1f, 1.0f,
     0.2f, 0.015f, 0.1f, 1.0f,
     0.2f, 0.015f, -0.1f, 1.0f,
    -0.2f, 0.015f, -0.1f, 1.0f,
    
    -0.2f, -0.015f, 0.1f, 1.0f,
     0.2f, -0.015f, 0.1f, 1.0f,
     0.2f, 0.015f, 0.1f, 1.0f,
    -0.2f, 0.015f, 0.1f, 1.0f,
    
    -0.2f, -0.015f,  0.1f, 1.0f,
    -0.2f,  0.015f,  0.1f, 1.0f,
    -0.2f,  0.015f, -0.1f, 1.0f,
    -0.2f, -0.015f, -0.1f, 1.0f
};

int Graph3D::UPDATE_CYCLE_CNT = 60;
int Graph3D::SHOW_CNT = 50;


Graph3D::Graph3D(string name) :
    currentIdx(0),
    currentCycle(0)
{
    this->name = name;
    
    company = "ADS";

    reader = new MDArchiveDataReader("/home/oculus/trading-desk-project/code/src/DataReaders/2014.01.02/" + company + ".h5");
    lastDone = reader->getLastDones();
    lastDoneCnt = reader->getLastDoneCnt();

    vertexShader = "#version 330\n"
                    "layout(location = 0) in vec4 position;\n"
                    "out vec4 color;\n"
                    "uniform mat4 projectionMat;\n"
                    "uniform mat4 modelViewMat;\n"
                    "uniform mat4 screenOffsetMat;\n"
                    "uniform mat4 rotationMat;\n"
                    "uniform mat4 offset;\n"
                    "uniform mat4 offsetGraph;\n"
                    "uniform mat4 rotateBlock;\n"
                    "uniform mat4 scale;\n"
                    "uniform mat4 rotateGraph;\n"
                    "uniform vec4 recolor;\n"
                    "void main(){\n"
                        "gl_Position = projectionMat * modelViewMat * screenOffsetMat * rotationMat * offsetGraph * rotateGraph * offset * rotateBlock * scale * position; \n"
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

Graph3D::~Graph3D() {
    delete reader;
}

void Graph3D::InitBuffers() {
    glGenBuffers(1, &blockBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, blockBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(block), block, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    cout << "[" << name << "] buffers loaded" << endl;
}

void Graph3D::InitProgram() {
    vector<GLuint> shaderList;
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, vertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, fragmentShader));
    shaderProgram = CreateProgram(shaderList);
    for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    
    cout << "[" << name << "] program initialized" << endl;
}

void Graph3D::UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection) {
    glUseProgram(shaderProgram);
        GLuint modelViewMatUnif = glGetUniformLocation(shaderProgram, "modelViewMat");
        glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(eyeModelview));
        GLuint projectionMatUnif = glGetUniformLocation(shaderProgram, "projectionMat");
        glUniformMatrix4fv(projectionMatUnif, 1, GL_FALSE, glm::value_ptr(eyeProjection));
    glUseProgram(0);
}

void Graph3D::SetPosition(glm::mat4 screenOffset, float rotX, float rotY){
    glm::mat4 rotationYMat = glm::rotate(rotY , glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 rotationXMat = glm::rotate(rotationYMat, rotX , glm::vec3(1.0, 0.0, 0.0));
    
    glUseProgram(shaderProgram);
        GLuint rotationUnif = glGetUniformLocation(shaderProgram, "rotationMat");
        glUniformMatrix4fv(rotationUnif, 1, GL_FALSE, glm::value_ptr(rotationXMat));

        GLuint screenOffsetUnif = glGetUniformLocation(shaderProgram, "screenOffsetMat");
        glUniformMatrix4fv(screenOffsetUnif, 1, GL_FALSE, glm::value_ptr(screenOffset));
    glUseProgram(0);
}

void Graph3D::Render() {
    glUseProgram(shaderProgram);
    bool changed = false;
    if (glIsEnabled(GL_DEPTH_TEST) == false) {
        glEnable(GL_DEPTH_TEST);
        changed = true;
    }
    
    double zOffset = 0.0f;
    double yOffset = 0.0f;
    int start = min(lastDoneCnt - 1, currentCycle);
    
    double sum = 0.0f;
    for (int i = start; i >= 1; --i)
        sum -= this->lastDone[i].price - this->lastDone[i - 1].price;
        
        for (int i = start; i >= max(1, start - SHOW_CNT); --i) {
            LastDone lastDone = this->lastDone[i];
            LastDone prevLastDone = this->lastDone[i - 1];
            float diff = lastDone.price - prevLastDone.price;
            
            GLuint scale = glGetUniformLocation(shaderProgram, "scale");
            float scaleFactorX = 1.5f + log((float)lastDone.volume) * 0.25f;
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactorX, 1.0f, 1.0f));
            glUniformMatrix4fv(scale, 1, GL_FALSE, glm::value_ptr(scaleMat));
            
            GLuint rotateBlock = glGetUniformLocation(shaderProgram, "rotateBlock");
            glm::mat4 rotateBlockMat = glm::rotate(-asin(diff / 0.2f) * 180.0f / 3.141592f, glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(rotateBlock, 1, GL_FALSE, glm::value_ptr(rotateBlockMat));
                    
            GLuint offset = glGetUniformLocation(shaderProgram, "offset");
            glm::mat4 offsetMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, yOffset - diff / 2.0f, zOffset));
            glUniformMatrix4fv(offset, 1, GL_FALSE, glm::value_ptr(offsetMat));
            
            GLuint rotateGraph = glGetUniformLocation(shaderProgram, "rotateGraph");
            glm::mat4 rotateGraphMat = glm::rotate(50.0f, glm::vec3(0.0, 1.0, 0.0));
            glUniformMatrix4fv(rotateGraph, 1, GL_FALSE, glm::value_ptr(rotateGraphMat));
            
            GLuint offsetGraph = glGetUniformLocation(shaderProgram, "offsetGraph");
            glm::mat4 offsetGraphMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.9f));
            glUniformMatrix4fv(offsetGraph, 1, GL_FALSE, glm::value_ptr(offsetGraphMat));
            
            GLuint color = glGetUniformLocation(shaderProgram, "recolor");
            glm::vec4 colorVec;
            
            yOffset -= diff;
            if (diff > 0.2f) diff = 0.2f;
            zOffset -= sqrt(0.2 * 0.2 - diff * diff) + 0.05f;

            if (diff > 0) {
                diff = diff / lastDone.price * 1000.0f;
                if (diff > 1.0f) diff = 1.0f;
                if (diff < 0.2f) colorVec = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
                else colorVec = glm::vec4(0.0f, diff, 0.0f, 1.0f);
            } else {
                diff = -diff / prevLastDone.price * 1000.0f;
                if (diff > 1.0f) diff = 1.0f;
                if (diff < 0.2f) colorVec = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
                else colorVec = glm::vec4(diff, 0.0f, 0.0f, 1.0f);
            }

            glUniform4fv(color, 1, glm::value_ptr(colorVec));


            glBindBuffer(GL_ARRAY_BUFFER, blockBuffer);
            glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glDrawArrays(GL_QUADS, 0, 16);
            glDisableVertexAttribArray(0);
        }
    
    if (changed) {
        glDisable(GL_DEPTH_TEST);
    }
    
    GLuint scale = glGetUniformLocation(shaderProgram, "scale");
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(scale, 1, GL_FALSE, glm::value_ptr(scaleMat));
    GLuint rotateBlock = glGetUniformLocation(shaderProgram, "rotateBlock");
    glm::mat4 rotateBlockMat = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(rotateBlock, 1, GL_FALSE, glm::value_ptr(rotateBlockMat));
    GLuint offset = glGetUniformLocation(shaderProgram, "offset");
    glm::mat4 offsetMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(offset, 1, GL_FALSE, glm::value_ptr(offsetMat));
    GLuint rotateGraph = glGetUniformLocation(shaderProgram, "rotateGraph");
    glm::mat4 rotateGraphMat = glm::rotate(50.0f, glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(rotateGraph, 1, GL_FALSE, glm::value_ptr(rotateGraphMat));
    GLuint offsetGraph = glGetUniformLocation(shaderProgram, "offsetGraph");
    glm::mat4 offsetGraphMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.9f));
    glUniformMatrix4fv(offsetGraph, 1, GL_FALSE, glm::value_ptr(offsetGraphMat));
    GLuint color = glGetUniformLocation(shaderProgram, "recolor");
    glm::vec4 colorVec = glm::vec4(1.0f, 1.0f, 1.0f, 0.2f);
    glUniform4fv(color, 1, glm::value_ptr(colorVec));
    glLineWidth(2.5);
    glBegin(GL_LINES);
        glVertex3f(0.0, sum, 0.0);
        glVertex3f(0.0, sum, zOffset - 1.0f);
    glEnd();
    
    glUseProgram(0);

    ++currentIdx;
    if (currentIdx % UPDATE_CYCLE_CNT == 0) {
        ++currentCycle;
    }
}