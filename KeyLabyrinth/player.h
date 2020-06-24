#if !defined(__PLAYER_H)
#define __PLAYER_H
#pragma once
#include <GLFW/glfw3.h>
#include "map.h"
#include "enemy.h"
#include "Camera.h"
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Player
{
private:

	bool isWalking = false;
	bool attOn = false;

	//jump logic
	bool canJump = true;
	bool jumping = false;
	float jumpVel = 0.0f;
	float jumpPower = 2.8f;
	float currentAngle = 180.0f;
	float myAttenuation = 1.0f;

	// camera
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraFrontX = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera* cam;
	

public:
	Player(Camera* cam);
	void handleJump();
	glm::vec3 getPos();
	void setXPos(float newX);
	void setYPos(float newY);
	void setZPos(float newZ);
	void setAngle(float angle);
	float getAttenuation();
	void setAttenuationOn(bool val);
	bool getAttenuationOn();
	void setAttenuation(float val);
	glm::vec3 getCamFront();
	glm::vec3 getCamUp();
	void setCamFront(glm::vec3 value);
	bool isJumping();
	void setWalking(bool val);
	bool getWalking();
	//void setCamFront(glm::vec3 value);
	void render(Shader objectShader);
	~Player();
};


#endif