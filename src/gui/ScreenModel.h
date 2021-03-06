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

extern const float mainScreen[32];
extern const float topScreen[32];
extern const float bottomScreen[32];
extern const float leftScreen[32];
extern const float rightScreen[32];

void InitBuffers();

#endif /* SCREENMODEL_H */