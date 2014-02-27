#ifndef ASKBIDBUBBLECHART_H
#define	ASKBIDBUBBLECHART_H

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "Visualisation.h"
#include "DataReaders/MDArchiveDataReader.h"

class AskBidBubbleChart : public Visualisation {    
        private:
            // Vertex data
            static float xaxis[32];
            static float yaxis[32];
            static float bubble[32];
            // Buffers
            GLuint xaxisBuffer;
            GLuint yaxisBuffer;
            GLuint bubbleBuffer;
            // For computation
            int cycle;
            int cycle2;
            int numCompanies;
            std::vector<float> askVolumes;
            std::vector<float> bidVolumes;
            std::vector<float> askPrices;
            std::vector<float> bidPrices;
            std::vector<MDArchiveDataReader*> readers;
            std::vector<int> retailStateCounts;
        public:
            AskBidBubbleChart(std::string name);
            ~AskBidBubbleChart();
            void InitBuffers();
            void InitProgram();
            void ComputeChange();
            void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection);
            void SetPosition(glm::mat4 screenOffset, float rotX, float rotY);
            void Render();
            
            void setCompanies(std::vector<std::string> companies);
            void drawCircle(float r, float cx, float cy, int segments, glm::vec3 colour);
};


#endif	/* ASKBIDBUBBLECHART_H */

