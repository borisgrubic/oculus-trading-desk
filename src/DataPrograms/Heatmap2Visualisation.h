#ifndef HEATMAP2_VISUALISATION_H
#define	HEATMAP2_VISUALISATION_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "Visualisation.h"
#include "DataReaders/MDArchiveDataReader.h"

class Heatmap2Visualisation : public Visualisation {    
        private:
            vector<string> companies;

            vector<int> retailStatesCnt;
            vector<RetailState*> retailStates;
            vector<MDArchiveDataReader*> readers;

            GLuint cellBuffer;
            static float cell[];

            int currentIdx;
            int currentCycle;
            static int UPDATE_CYCLE_CNT;

        public:
            Heatmap2Visualisation(string);
            ~Heatmap2Visualisation();
            void InitBuffers();
            void InitProgram();
            void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection);
            void SetPosition(glm::mat4 screenOffset, float rotX, float rotY);
            void Render();
            
            float getBid(RetailState, int);
            float getAsk(RetailState, int);
};


#endif

