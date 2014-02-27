#ifndef GRAPH_3D_H
#define	GRAPH_3D_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "Visualisation.h"
#include "DataReaders/MDArchiveDataReader.h"

class Graph3D : public Visualisation {    
        private:
            string company;

            int lastDoneCnt;
            LastDone* lastDone;
            MDArchiveDataReader* reader;

            GLuint blockBuffer;
            static float block[];

            int currentIdx;
            int currentCycle;
            static int UPDATE_CYCLE_CNT;
            static int SHOW_CNT;

        public:
            Graph3D(string);
            ~Graph3D();
            void InitBuffers();
            void InitProgram();
            void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection);
            void SetPosition(glm::mat4 screenOffset, float rotX, float rotY);
            void Render();
};


#endif

