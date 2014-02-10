#ifndef SCREENMODEL_H
#define SCREENMODEL_H

#include <string>

extern const std::string screenVertexShader;
extern const std::string screenFragmentShader;

extern GLuint mainScreenBuffer;
extern GLuint topScreenBuffer;
extern GLuint bottomScreenBuffer;
extern GLuint leftScreenBuffer;
extern GLuint rightScreenBuffer;
extern GLuint testGraphBuffer;

extern const float mainScreen[32];
extern const float topScreen[32];
extern const float bottomScreen[32];
extern const float leftScreen[32];
extern const float rightScreen[32];
extern const float testGraph[64];

void InitBuffers();
void InitTestGraphBuffers();

#endif /* SCREENMODEL_H */