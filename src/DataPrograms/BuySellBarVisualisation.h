#ifndef BUYSELLBARVISUALISATION_H
#define	BUYSELLBARVISUALISATION_H

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "Visualisation.h"
#include "DataReaders/CSVDataReader.h"
#include "DataReaders/DataReader.h"
#include "DataTypes/CSVDataType.h"
#include "SourceReaders/RandomCSVSimpleReader.h"

class BuySellBarVisualisation : public Visualisation {    
        private:
            std::string name;
            // Shaders
            std::string vertexShader;
            std::string fragmentShader;
            // Vertex data
            static float divisor[32];
            static float bar[];
            // Buffers
            GLuint divisorBuffer;
            GLuint barBuffer;
            // Shader program
            GLuint shaderProgram;
            // Readers
            CSVDataReader* reader;
            // For computation
            vector<float> companyOffsets;
            vector<float> companyScales;
            vector<glm::vec3> companyColours;
            int cycle;
        public:
            BuySellBarVisualisation(std::string name);
            ~BuySellBarVisualisation();
            void InitBuffers();
            void InitProgram();
            void ComputeChange();
            void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection);
            void SetPosition(glm::mat4 screenOffset);
            void Render();
};


#endif	/* BUYSELLBARVISUALISATION_H */

