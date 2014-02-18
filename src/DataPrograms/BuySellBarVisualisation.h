#ifndef BUYSELLBARVISUALISATION_H
#define	BUYSELLBARVISUALISATION_H

#include <cstdlib>
#include <vector>

class BuySellBarVisualisation : public Visualisation {    
        private:
            std::string name;
            // Shaders
            const std::string vertexShader;
            const std::string fragmentShader;
            // Vertex data
            const float divisor[32];
            const float bar[32];
            // Buffers
            GLuint divisorBuffer;
            GLuint barBuffer;
            // Shader program
            GLuint shaderProgram;
            // Readers
            CSVDataReader* reader;
        public:
            BuySellBarVisualisation(std::string name);
            void InitBuffers();
            void CreateProgram();
            void ComputeChange();
            void Render();
};


#endif	/* BUYSELLBARVISUALISATION_H */

