
#ifndef LINEGRAPH_H
#define	LINEGRAPH_H


#include <cstdlib>
#include <vector>
#include <algorithm>
#include "Visualisation.h"
#include "DataReaders/CSVDataReader.h"
#include "DataReaders/DataReader.h"
#include "DataTypes/CSVDataType.h"
#include "SourceReaders/RandomCSVSimpleReader.h"


class LineGraph : public Visualisation {
private:
            std::string name;
            // Shaders
            std::string vertexShader;
            std::string fragmentShader;
            // Vertex data
            static float xAxis[32];
            static float yAxis[32];
            static float points[160];
            // Buffers
            GLuint xAxisBuffer;
            GLuint yAxisBuffer;
           
           
            GLuint pointsBuffer;
            // Shader program
            GLuint shaderProgram;
            // For computation
            int cycle;
            std::vector<float> companyPrice; 
            CSVDataReader* reader;
        public:
            LineGraph(std::string name);
            ~LineGraph();
            void InitBuffers();
            void InitProgram();
            void ComputeChange();
            void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection);
            void SetPosition(glm::mat4 screenOffset, float rotX, float rotY);
            void Render();
            
   

};

#endif	/* LINEGRAPH_H */

