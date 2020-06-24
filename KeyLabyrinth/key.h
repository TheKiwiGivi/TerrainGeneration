#if !defined(__KEY_SYSTEM_H)
#define __KEY_SYSTEM_H
#pragma once
#include "map.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


class Map; //forward declaration of Map

class Key
{
private:
	Map* map;
	bool animated=false;
	bool found=false;
	glm::vec3 position;
public:
	Key(Map* input, glm::vec3 pos);
	void checkFound(glm::vec3 playerPos);	//checks if key is found and updates if yes

	bool isAnimated();
	bool isFound();

	void setAnimated(bool value);
	void setFound(bool value);
};

#endif