#ifndef DISPLAY_H
#define DISPLAY_H

#include <glfw3.h>
#include <glm/glm.hpp>

void Init(bool displayOnScreen);
void InitBuffers();
void RunDisplay();
void ClearWindowDisplay();

GLFWmonitor* getMonitorAtPosition(glm::ivec2 & position);

void draw();
void update();

#endif /* DISPLAY_H */
