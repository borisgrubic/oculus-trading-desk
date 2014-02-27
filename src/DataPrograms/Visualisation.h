#ifndef VISUALISATION_H
#define	VISUALISATION_H

#include <GL/glew.h>
#include <cstdlib>
#include <vector>
#include <string>

class Visualisation {
    protected:    
        std::string name;
        std::string vertexShader;
        std::string fragmentShader;
        GLuint shaderProgram;
    
    public:
        virtual void InitBuffers() = 0;
        virtual void InitProgram() = 0;
        virtual void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection) = 0;
        virtual void SetPosition(glm::mat4 screenOffset, float rotX, float rotY) = 0;
        virtual void Render() = 0;
        virtual ~Visualisation() {}
};


#endif	/* VISUALISATION_H */

