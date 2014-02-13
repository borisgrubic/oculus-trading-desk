#ifndef SCREENMODEL_H
#define SCREENMODEL_H

#include <iostream>
#include <string>

extern const std::string screenVertexShader;
extern const std::string screenFragmentShader;

extern GLuint mainScreenBuffer;
extern GLuint topScreenBuffer;
extern GLuint bottomScreenBuffer;
extern GLuint leftScreenBuffer;
extern GLuint rightScreenBuffer;
extern GLuint testGraphBuffer;

extern GLuint vis1Buffer;

extern const float mainScreen[32];
extern const float topScreen[32];
extern const float bottomScreen[32];
extern const float leftScreen[32];
extern const float rightScreen[32];
extern const float testGraph[32];

extern const float vis1[256];

void InitBuffers();
void InitTestGraphBuffers();
void InitVisualisationBuffers();

#endif /* SCREENMODEL_H */