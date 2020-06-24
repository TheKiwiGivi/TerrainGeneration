#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include "enemy.h"


extern int enemyID;

Enemy::Enemy(Map* inMap, glm::vec3 inPos)
{


    this->map = inMap;
    this->position = inPos;
    //std::cout << "CUBECPUTNER: " << map->getCubeCounter() << "\n\n\n\n";
    std::srand(time(NULL));

    state = roaming;
    start = clock();

    eId = enemyID++;
    std::cout << "enemy " << eId << " created\n";


}

//does something based on what state the enemy is in.
void Enemy::activate(glm::vec3 playerPos)
{
    glm::vec3 towardPlayer = playerPos - position;
    float distance = glm::length(towardPlayer);

//std::cout << "TOWARD = " << position.x << "\n\n\n";
	chase(towardPlayer);
	//std::cout << towardPlayer.y<<" ";
	return;

    //if player is within 3 units, chase them
    if (distance < 1.0f)
    {
        state = chasing;
    }
    else if (distance < 7.0f)    //if they get past this distance, go back to roaming randomly
    {
        state = roaming;
    }

    switch (state)
    {
    case(roaming):
    {
        roam();             //roam randomly
        break;
    }
    case(chasing):
    {
        chase(towardPlayer);   //chase player
        break;
    }
    default: break;
    }
}

void Enemy::chase(glm::vec3 towardPlayer)
{
    //maybe add some LOS stuff here?
    step(towardPlayer);
}



//sets the enemy's rotation to the angle between parameter vector and the posiive z axis, the default facing
void Enemy::face(glm::vec3 direction)
{
    //normalize the input vector
    direction = glm::normalize(direction);
    //make vector that is the "default" facing, the Z axis
    glm::vec3 zVector = glm::vec3(0.0f, 0.0f, 1.0f);

    //angle to rotate(in radians)
    float angle = glm::acos(glm::dot(zVector, direction));

    rotation = glm::degrees(angle);
}

//when the enemy is in its roaming state, it will randomly walk in a set direction until either the 
//time it set for this roam elapses, or until it hits a wall, at which point it will change directions
void Enemy::roam()
{
    std::clock_t now = clock();
    double elapsed = (now - start) / (double)CLOCKS_PER_SEC;
    //if more than roamTime seconds have passed since clock was reset
	std::cout << elapsed << "wlelele\n";
    if (elapsed > 10) 
    {
        //std::cout << elapsed << "\n";
        start = clock();             //reset clock
        roamTime = 1 + (rand() % 3); //generate random amount of time between 1 and 4 seconds

        //find new direction
        newDirection();
    }
    else                            //if time has not elapsed
    {
        //if we would hit a wall
        //if (!map->notWall(position.x + roamDirection.x, position.z + roamDirection.z))
        {
           // std::cout << roamDirection.x << " " << roamDirection.z << "\n";
            //find new direction
            //newDirection();
        }
        step(roamDirection);        //keep walking that way
    }

    


}

void Enemy::newDirection()
{
    float randX = (float)(rand() % 10) + 1; //generate random float between -1 and 1
    if (rand() % 2)
    {
        randX = -randX;
    }
    float randZ = (float)(rand() % 10) + 1; //generate random float between -1 and 1
    if (rand() % 2)
    {
        randZ = -randZ;
    }

    roamDirection = glm::vec3(randX, 0.0f, randZ);

    //normalize vector
    roamDirection = glm::normalize(roamDirection);
    //muliply it by enemy's speed
    roamDirection = roamDirection * enemySpeed;
}


void Enemy::render(Shader objectShader)
{
    glm::mat4 model3 = glm::mat4(1.0f);
    model3 = glm::translate(model3, position); // translate it down so it's at the center of the scene
    model3 = glm::scale(model3, glm::vec3(0.033f, 0.033f, 0.033f));	// it's a bit too big for our scene, scale it down by a factor of 3
    model3 = glm::rotate(model3, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate around the y axis if it has turned


    objectShader.setMat4("model", model3);

    //ourModel2.Draw(objectShader);
}

//takes a step along provided vector
void Enemy::step(glm::vec3 direction)
{
    face(direction); //update facing


    //normalize vector
    direction = glm::normalize(direction);
    //muliply it by enemy's speed
    direction = direction * enemySpeed;

	
	//std::cout << "DIRECTION " << direction.x << "-" << direction.y<< "\n";
    //if this place is not a wall
    //if (map->notWall(position.x + direction.x, position.z + direction.z))
    {
        position.x += direction.x;
        position.z += direction.z;
		position.y = map->getYPos(position.x, position.z);
		//std::cout << newYPosition << "\n";
		


    }
	if (position.x < 0.0f) {
		position.x = 0.0f;
		
		roamDirection.x *= -1;
	}
	if (position.y < 0.0f) {
		position.y = 0.0f;
		roamDirection.y *= -1;
	}




}

//Debug, should not be used to actually set the position D:
void Enemy::setPosition(glm::vec3 input)
{
    this->position = input;
}

glm::vec3 Enemy::getPosition()
{
    return position;
}

float Enemy::getRotation()
{
    return rotation;
}

//DEBUG
void Enemy::toggleState(EnemyState state)
{
    this->state = state;
}