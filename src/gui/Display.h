#ifndef DISPLAY_H
#define DISPLAY_H

#include <glfw3.h>
#include <glm/glm.hpp>

void Init(bool displayOnScreen);
void RunDisplay();
void ClearWindowDisplay();
void UpdateView(glm::vec3 eyeModelView, glm::vec3 eyeProjection);

GLFWmonitor* getMonitorAtPosition(glm::ivec2 & position);

void draw();
void update();

#endif /* DISPLAY_H */
