#if !defined(__MAP_H)
#define __MAP_H
#pragma once

#include <GL/glew.h>
#include "consts.h"
#include "key.h"
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include "chest.h"
#include "Shader.h"
#include "heightmap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/matrix_transform.hpp>

class Key; //forward declaration of Key

class Map
{
private:
	//VAOs
	unsigned int VAO1, VAO2;
	unsigned int VAO[128*129];

	//map dimensions
	int mapLengthX = 7;
	int mapLengthZ = 10;

	//used to make sure tiles are registered correctly
	float bonusX = 0.0f;
	float bonusZ = 0.0f;

	HeightMap* hm;

	//rain
	float rainHeight = 2.0f;
	float rainMin = 0.0f;
	float raindropSpeed = 0.02f;
	bool rainActive = true;
	bool cloudActive = true;
	int cloudAmount[100];

	//cloud
	float rainDensity = 500.0f;
	int cloudCounter = 0;
	glm::vec3 cloudPositions[100];
	glm::vec3 cloudSize[100];

	//previous location of the plauer
	int lastX = 0;
	int lastZ = 0;

	//current location of the player
	int currentX = 0;
	int currentZ = 0;

	int animation = 0;
	int currentAmountKey = 0;
	int totalTorches = 0;
	
	glm::vec3 sunStartPosition = glm::vec3(0, 1.0f, 0);
	glm::vec3 sunPosition =sunStartPosition;
	float sunPower = 0.055f;
	glm::vec3 sunStartVel = glm::vec3(sunPower, sunPower, sunPower);
	glm::vec3 sunVel = sunStartVel;
	bool sunActive = true;
	//gravity
	float gravity =0.0015f;

	//offset I found visually suitable for torch light
	float torchOffset = 0.58f;
	float sunAngle = 0.0f;
	//unsigned int texture1, texture2;
	
	float dimX = 0.05f;
	float dimZ = 0.05f;

	//positions
	glm::vec3 wallPositions[15][15];
	glm::vec3 torchPositions[AMOUNT_TORCHES];
	glm::vec3 spikePositions[AMOUNT_SPIKES];
	glm::vec3 chestPositions[AMOUNT_CHESTS];

	int torchType[100];

	//counters
	int chestCounter = 0;
	int cubeCounter = 0;
	int torchCounter = 0;
	int spikeCounter = 0;

	

	bool createMode = true;

	bool walkedVal[10000];
	bool rememberVal[10000];
	bool rememberBlue[10000];
	bool beachVal[10000];
	bool snowVal[10000];
	int remCounter = 0;
	glm::vec3 wierdColor;
	glm::vec3 beachColor = glm::vec3((float(248) / 255), (float(240) / 255), (float(10) / 255));
	glm::vec3 snowColor = glm::vec3((float(101) / 255), (float(67) / 255), (float(33) / 255));
	float colorTime = 1.0f;

	//position of enemy
	glm::vec3 enemyPos;

	Key* keys[AMOUNT_KEYS];

	//Player* player;

public:
	glm::vec3 cubePositions[NUMOFCUBES];

	Map(Shader ourShader, HeightMap* heightMap);
	void render(unsigned int floorVAO, unsigned int spikeVAO, Shader ourShader, HeightMap* heightMap);
	void drawTorches(Shader objectShader, int current);
	void initializeWall(Shader ourShader);
	glm::vec3 convertToGridCoordinates(glm::vec3 input);
	glm::vec3 convertToSceneCoordinates(glm::vec3 input);
	bool notWall(float x, float z);
	//void initChestPositions();
	void checkFoundKeys(glm::vec3 playerPos); //calls all keys checkFound
	bool winGame();
	void initMapfiles();
	void foundKey();
	void updateRaindrops();
	//getters
	glm::ivec2 getMapSize();
	int getCubeCounter();
	int getCurrentX();
	int getCurrentZ();
	int getMapLengthX();
	int getMapLengthZ();
	float getBonusX();
	glm::vec3 getSunPos();
	float getBonusZ();
	glm::vec3 getSpikePos(int nr);
	int getSpikeCount();
	int getTotalKeys();
	int getAmountKeys();
	glm::vec3 getTorchPos(int torchId);
	int getTorchCount();
	int getTorchType(int torchId);
	void makeLight(glm::vec3 pos);
	void setTorchPos(int i, glm::vec3 pos);
	void toggleRain(bool val);
	void createRaindrops();
	void setRainDensity(float density);
	float setRainDensity();
	float getYPos(float xPos, float zPos);
	bool getSunActive();
	void setSunActive(bool val);
	void moveSun();
	void setCloudActive(bool val);
	float getDimX();
	float getDimZ();
	void makeCloud(glm::vec3 pos, glm::vec3 cSize);
	float mapping(float x, float in_min, float in_max, float out_min, float out_max);
	~Map();
};
#endif