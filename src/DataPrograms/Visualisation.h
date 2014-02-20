#ifndef VISUALISATION_H
#define	VISUALISATION_H

#include <GL/glew.h>
#include <cstdlib>
#include <vector>
#include <string>

class Visualisation {
    
    std::string name;
    
    const std::string vertexShader;
    const std::string fragmentShader;
    GLuint shaderProgram;
    
    public:
        virtual void InitBuffers() = 0;
        virtual void InitProgram() = 0;
        virtual void UpdateView(glm::mat4 eyeModelview, glm::mat4 eyeProjection) = 0;
        virtual void Render() = 0;   
        virtual ~Visualisation() {}
};


#endif	/* VISUALISATION_H */

