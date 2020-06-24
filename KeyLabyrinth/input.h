#if !defined(__INPUT_H)
#define __INPUT_H
#pragma once
#include <GLFW/glfw3.h>
#include "map.h"
#include "enemy.h"
#include "player.h"

void processInput(GLFWwindow *window, Map* map, Player* player);


#endif