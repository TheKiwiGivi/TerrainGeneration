#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include "mouse.h"


#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
extern int cameraMode;
extern bool isDead;
extern bool wonTheGame;
extern bool useCursor;


void mouse_callback2(GLFWwindow* window, double xpos, double ypos, Player* player)
{
	
	if (useCursor || isDead || wonTheGame) {
		return;
	}
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	//disable up/down if camera mode is top down
	if (player->getWalking()) {
		//yoffset = 0;
	}
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; //sensitivity TODO: make setting
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	
	player->setAngle(-yaw+90.0f);

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		player->setCamFront(glm::normalize(front));
	
	//cameraFrontX = glm::normalize(front);
}