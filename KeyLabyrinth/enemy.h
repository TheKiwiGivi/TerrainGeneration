#if !defined(__ENEMY_H)
#define __ENEMY_H
#pragma once
#include "consts.h"
#include "map.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <ctime>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "Shader.h"



class Enemy
{
private:
	
	int eId;

	//vector to enemy's current position relative to the scene, used for rendering
	glm::vec3 position;
	float enemySpeed = 0.01f;      //enemy speed
	float rotation = 0.0f;          //rotation of model on the y axis (in degrees), used for turning the enemy

    glm::vec3 roamDirection = glm::vec3(0.0f, 0.0f, 1.0f);  //direction enemy is currently roaming in, used in roam();
    std::clock_t   start;                                   //when current roam routine was started
    int roamTime = 3;                                       //how long to roam for

	Map* map;	//pointer to map enemy is on, used to "see" data about level.

public:
	enum EnemyState { roaming, chasing };
	EnemyState state;
	Enemy(Map* inMap, glm::vec3 inPos); //constructor, with pointer to map 

    void activate(glm::vec3 playerPos); //checks what state enemy is in and calls appropriate funcion
    void chase(glm::vec3 playerPos);    //runs straight at the player
	void roam();                        //roams randomly
    void face(glm::vec3 direction); //changes what direction the enemy is facing
    void newDirection();
	void render(Shader objectShader);
    void step(glm::vec3 direction);
	void setPosition(glm::vec3 input); //Debug
	glm::vec3 getPosition();
	float getRotation();

    //DEBUG
    void toggleState(EnemyState enemyState);
};
#endif