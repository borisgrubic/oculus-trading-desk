#ifndef VISUALISATION_H
#define	VISUALISATION_H

#include <cstdlib>
#include <vector>

class Visualisation {
    
    std::string name;
    
    const std::string vertexShader;
    const std::string fragmentShader;
    GLuint shaderProgram;
    
    public:
        Visualisation(std::vector<std::string> companies);
        virtual void InitBuffers();
        virtual void CreateProgram();
        virtual void Render();   
    
};


#endif	/* VISUALISATION_H */

