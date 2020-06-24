#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif 
#include "key.h"

extern glm::vec3 cameraPos;

bool wonGame = false; // where to put this?


Key::Key(Map* input, glm::vec3 pos)
{
	map = input;
	position = pos;
}

void Key::checkFound(glm::vec3 playerPos)
{
	if (!found && !animated){
		//subract player position from chest's position
		glm::vec3 diff = { position.x - playerPos.x, position.y - playerPos.y, position.z - playerPos.z };

	//find the distance as a value
	float distance = glm::length(diff);

	//if that distanc
	if (distance < CHEST_DISTANCE_TOLERANCE)
	{ //if the key is not found or animated, and the camera is close to the key
		std::cout << "Key found! " << distance << "\n";
		animated = true;
		map->foundKey();
		//call foundKey
	//else display message?
	//std::cout << "No key found. \n";
	}
}
}



	//getters
	bool Key::isAnimated()
	{
		return this->animated;
	}
	bool Key::isFound()
	{
		return this->found;
	}

	//setters
	void Key::setAnimated(bool value)
	{
		this->animated = value;
	}

	void Key::setFound(bool value)
	{
		this->found = value;
	}