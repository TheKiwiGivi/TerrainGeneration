#if !defined(__MOUSE_H)
#define __MOUSE_H
#pragma once
#define GLFW_INCLUDE_NONE
#include "player.h"
#include <GLFW/glfw3.h>

void mouse_callback2(GLFWwindow* window, double xpos, double ypos, Player* player);

#endif