#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif 
#define GLFW_INCLUDE_NONE
#include <iostream>
#include "player.h"

extern float deltaTime;


Player::Player(Camera* cam1) {
	//std::cout << cameraPos.x << " " << cameraPos.z<<"\n\n";
	cam = cam1;

}

Player::~Player() {

}

void Player::handleJump() {
	if (jumping) {
		jumping = false;
		canJump = false;
		jumpVel = jumpPower;
		cameraPos.y += 0.01f;
	}
	if (cameraPos.y > 0.0f) {

		cameraPos.y += jumpVel * deltaTime;
		jumpVel -= 9.8 * deltaTime;

	}
	else {
		canJump = true;
		cameraPos.y = 0.0f;
	}
}

glm::vec3 Player::getPos() {
	return cameraPos;
}

void Player::setXPos(float newX) {
	cameraPos.x = newX;
	
}
void Player::setYPos(float newY) {
	
		cameraPos.y = newY;
		
	
}



void Player::setZPos(float newZ) {
	cameraPos.z = newZ;
}

void Player::setAngle(float angle) {
	currentAngle = angle;
}

float Player::getAttenuation() {
	return myAttenuation;
}
void Player::setAttenuation(float val) {
	myAttenuation = val;
}

void Player::setAttenuationOn(bool val) {
	attOn = val;
}
bool Player::getAttenuationOn() {
	return attOn;
}

glm::vec3 Player::getCamFront() {
	return cameraFront;
}
glm::vec3 Player::getCamUp() {
	return cameraUp;
}
void Player::setCamFront(glm::vec3 value) {
	cameraFront = value;
}

bool Player::isJumping() {
	return jumping;
}

void Player::setWalking(bool val) {
	isWalking = val;
}

bool Player::getWalking() {
	return isWalking;
}

void Player::render(Shader objectShader) {
	//render player!
	objectShader.use();

	glm::mat4 model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(0.0f, -0.3f, 0.0f) + cameraPos); // translate it down so it's at the center of the scene
	model2 = glm::scale(model2, glm::vec3(0.033f, 0.033f, 0.033f));	// it's a bit too big for our scene, so scale it down

	model2 = glm::rotate(model2, glm::radians(currentAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	objectShader.setMat4("model", model2);

}


