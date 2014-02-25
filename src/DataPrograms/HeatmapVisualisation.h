#ifndef HEATMAP_VISUALISATION_H
#define	HEATMAP_VISUALISATION_H

#include "Visualisation.h"
#include "DataReaders/MDArchiveDataReader.h"

class HeatmapVisualisation : public Visualisation {    
        private:
            vector<string> companies;

            vector<int> retailStateCnt;
            vector<RetailState*> retailStates;
            vector<MDArchiveDataReader*> readers;

            GLuint cellBuffer;
            static float cell[];

            int currentCycle;
            static int UPDATE_CYCLE_CNT = 20;

        public:
            HeatmapVisualisation(vector<string> companies);
            ~HeatmapVisualisation();
            void InitBuffers();
            void InitProgram();
            void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection);
            void SetPosition(glm::mat4 screenOffset, float rotX, float rotY);
            void Render();
};


#endif

