#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif 
#include "map.h"

//extern glm::vec3 cameraPos;
extern glm::vec3 enemyPositions[];
extern int selectedMap;
extern int totalMaps;
extern float deltaTime;
//extern int enemyCounter;


extern float iter;

Map::Map(Shader ourShader, HeightMap* heightMap)
{
	//make sun 
	hm = heightMap;
	torchType[torchCounter] = 0;
	torchPositions[torchCounter] = sunPosition;
	torchCounter++;
	std::cout << "Sun: " << torchCounter - 1 << "\n";
	//set map dimensions
	mapLengthX = heightMap->getWidth();
	mapLengthZ = heightMap->getHeight();


	initializeWall(ourShader);
	//create VAOs


	cloudCounter++;
	cloudPositions[0] = glm::vec3(1, 1, 1);
	cloudSize[0] = glm::vec3(1, 0.01, 1);
	createRaindrops();


/*
	//read map from file
	std::ifstream myReadFile;
	std::string mapName = "map_";
	mapName.append(std::to_string(selectedMap));
	mapName.append(".txt");
	myReadFile.open(mapName);

	if (myReadFile.is_open())
	{
		//map dimensions
		myReadFile >> mapLengthX;
		myReadFile >> mapLengthZ;
		std::cout << "CHECKING\n";
		//used to make sure tiles are calculated correctly
		if (mapLengthX % 2 == 0)
		{
			bonusX = 0.5f;
			enemyPos.x += bonusX;
		}
		if (mapLengthZ % 2 == 0)
		{
			bonusZ = 0.5f;
			enemyPos.z += bonusZ;
		}


		//looping through map
		for (int i = 0; i < mapLengthZ; i++)
		{
			for (int j = 0; j < mapLengthX; j++)
			{
				int temp;
				myReadFile >> temp;
				//wall or wall+torch
				if (0)
				{

					//creating position
					int posX = j - mapLengthX / 2;
					int posZ = i - mapLengthZ / 2;

					//check if torch. add offset based on where torch will be placed
					if (temp == 2)
					{
						//torchType[torchCounter] = temp;
						//torchPositions[torchCounter++] = glm::vec3(posX + bonusX, 0.1f, posZ + bonusZ + torchOffset);
					}
					else if (temp == 3)
					{
						//torchType[torchCounter] = temp;
						//torchPositions[torchCounter++] = glm::vec3(posX + bonusX + torchOffset, 0.1f, posZ + bonusZ);
					}
					else if (temp == 4)
					{
						//torchType[torchCounter] = temp;
					//	torchPositions[torchCounter++] = glm::vec3(posX + bonusX, 0.1f, posZ + bonusZ - torchOffset);
					}
					else if (temp == 5)
					{
						//torchType[torchCounter] = temp;
						//torchPositions[torchCounter++] = glm::vec3(posX + bonusX - torchOffset, 0.1f, posZ + bonusZ);
					}
					else if (temp == 6)
					{
						//player pos

						//cameraPos.x = posX + bonusX;
						//cameraPos.z = posZ + bonusZ;

					}
					else if (temp == 7)
					{
						//chestPositions[chestCounter++] = glm::vec3(posX + bonusX, -0.5f + 0.15f, posZ + bonusZ);
					}
					else if (temp == 8)
					{
						//enemyPositions[enemyCounter++] = glm::vec3(posX + bonusX, -0.5f, posZ + bonusZ);

					}
					else if (temp == 9)
					{
						//spikePositions[spikeCounter++] = glm::vec3(posX + bonusX, -0.5f, posZ + bonusZ);
					}


					//create wall
					if (temp != 6 && temp != 7 && temp != 8 && temp != 9)
					{
						std::cout << temp << "\n";
						float reverseBonusX = 0.0f;
						if (bonusX == 0.0f) {
							reverseBonusX = 0.5f;
						}
						float reverseBonusZ = 0.0f;
						if (bonusZ == 0.0f) {
							reverseBonusZ = 0.5f;
						}
						cubePositions[cubeCounter++] = glm::vec3(int(posX), 0.0f, int(posZ));

						std::cout << cubePositions[cubeCounter - 1].x << " POSITION " << cubePositions[cubeCounter - 1].z << "count: " << cubeCounter << std::endl;
						//std::cout << cubePositions[i].x + mapLengthX / 2 << " POS " << cubePositions[i].z + mapLengthZ / 2 << std::endl;
						//std::cout << bonusX << " " << bonusZ << std::endl;
					}
				}
			}
		}
	}
	else
	{
		std::cout << "Map not found.";
	}
	myReadFile.close();
	//giving shader the amount of torches in map
	ourShader.setInt("totalTorches", torchCounter);
	//initChestPositions();

	//calculating positions for walls, and moving things from 1d cubePositions array to 2d wallPositions array
	for (int i = 0; i < mapLengthZ; i++)
	{
		for (int j = 0; j < mapLengthX; j++)
		{
			//copy the next entry
			glm::vec3 currentPosition = cubePositions[i * 10 + j];
			//move it into the appropriate index in the array
			wallPositions[i][j] = glm::vec3(int(currentPosition.x + float(mapLengthX) / 2), 0.0f, int(currentPosition.z + float(mapLengthZ) / 2));

		}
	}
	float angle = 0.5f;
	float angle2 = 0.1f;
	float angle3 = angle2*2;
	*/
	int counter = 0;
	for (int i = 0; i < heightMap->getHeight(); i++) {
		for (int j = 0; j < heightMap->getWidth(); j++) {
			float angle = heightMap->getData(i, j);
			if (angle >= 1.0f) {
				//std::cout << "WTF????? 1!! : " << angle << "\n\n\n";
			}
			
			

				float heightMapLowest = heightMap->getMin2();
				float heightMapHighest = heightMap->getMax2();

				float input_start = heightMapLowest; // The lowest number of the range input.
				float input_end = heightMapHighest; // The lowest number of the range input.
				float output_start = 0.0f; // The lowest number of the range output.
				float output_end = 1.0f; // The largest number of the range ouput.

				float input = angle; // Input value.
				float output = 0.0f;

				// std::cout << "min is" << heightMapLowest << " and max is " << heightMapHighest << "\n";



				//angle = output;

				output = mapping(input, heightMapLowest, heightMapHighest, 0.0f, 1.0f);

				if (angle == output) {
					//std::cout << "same! " << angle << " - " << output << "\n";
				}
				//if (output > 0.0f)
					//angle = output;

				if (angle <= 0.0f) angle = 0.0f;
				if (angle >= 1.0f) angle = 1.0f;
				//std::cout << "angur " << angle << "\n";

				//if (angle <= 0.25f) angle = 0.25f;
			

			float realAngle = angle / 2;

			float right = 0.0f; 
			float top = 0.0f; 
			float left = 0.0f; 
			float bottom = 0.0f;

			float topRight = 0.0f;
			float topLeft = 0.0f;
			float bottomRight = 0.0f;
			float bottomLeft = 0.0f;
			
			 right = heightMap->getData(i, j + 1);
			 top = heightMap->getData(i + 1, j);
			 left = heightMap->getData(i, j - 1);
			 bottom = heightMap->getData(i - 1, j);

			 //

			 float right2 = heightMap->getData(i, j + 2);
			 float top2 = heightMap->getData(i + 2, j);
			 float left2 = heightMap->getData(i, j - 2);
			 float bottom2 = heightMap->getData(i - 2, j);

			 float right3 = heightMap->getData(i+1, j - 2);
			 float top3 = heightMap->getData(i - 2, j+1);
			 float left3 = heightMap->getData(i-1, j + 2);
			 float bottom3 = heightMap->getData(i + 2, j-1);

			 //


			 

			 topRight = heightMap->getData(i + 1, j + 1);
			 topLeft = heightMap->getData(i + 1, j - 1);
			 bottomRight = heightMap->getData(i - 1, j + 1);
			 bottomLeft = heightMap->getData(i - 1, j - 1);
			 
			 


			 float thresh = 0.45f;
			 float snowTresh = 0.75f;
			 beachVal[counter] = false;
			 rememberBlue[counter] = false;
			 rememberVal[counter] = false;
			 snowVal[counter] = false;
			 //if ((right < thresh || left < thresh || top < thresh || bottom < thresh || topRight< thresh || topLeft< thresh || bottomRight< thresh || bottomLeft< thresh)) {
			 if (heightMap->getBiome() == 3) {
				 float cTresh = 0.25f;
				 if (angle <= cTresh) {
					 rememberBlue[counter] = true;
				 }
				 else if ((right <= cTresh || left <= cTresh || top <= cTresh || bottom <= cTresh) && angle > cTresh) {
					 rememberVal[counter] = true;
				 }
				 else if ((right2 <= cTresh || left2 <= cTresh || top2 <= cTresh || bottom2 <= cTresh || top3 <= cTresh || top3 <= cTresh || bottom3 <= cTresh || bottom3 <= cTresh)) {

					 beachVal[counter] = true;
				 }
			 }
			 else if (heightMap->getBiome() == 1 && angle <= 0.50f && angle >= 0.70f) {

			 }
			 else {

				 if (angle <= heightMap->getHighTreshold()) {
					 float cTresh = heightMap->getTreshold();
					 if (angle <= cTresh) {
						 rememberBlue[counter] = true;
					 }
					 else if ((right <= cTresh || left <= cTresh || top <= cTresh || bottom <= cTresh) && angle > cTresh) {
						 rememberVal[counter] = true;
					 }
					 else if ((right2 <= cTresh || left2 <= cTresh || top2 <= cTresh || bottom2 <= cTresh || top3 <= cTresh || top3 <= cTresh || bottom3 <= cTresh || bottom3 <= cTresh)) {

						 beachVal[counter] = true;
					 }


				 }
				 else if (heightMap->getBiome() == 1 && angle >= snowTresh) {
					 // std::cout << "SNOW\n";
					 snowVal[counter] = true;
				 }
			 }
			// std::cout << angle << " ";
			 
			// }
			 

			
			float nope = 0.0f;
			float newRight = (angle + right) / 2;
			float newLeft = (angle + left) / 2;
			float newTop = (angle + top) / 2;
			float newBottom = (angle + bottom) / 2;

			float newTopRight = (angle + topRight + top + right) / 4;
			float newTopLeft = (angle + topLeft + top + left) / 4;
			float newBottomRight = (angle + bottomRight + bottom + right) / 4;
			float newBottomLeft = (angle + bottomLeft + bottom + left) / 4;



			//first triangle
			glm::vec3 x = glm::vec3(-0.5f*dimX, newBottomLeft, -0.5f*dimZ);
			glm::vec3 z = glm::vec3(0.5f*dimX, newTopLeft, -0.5f*dimZ);
			glm::vec3 y = glm::vec3(0.5f*dimX, newTopRight, 0.5f*dimZ);

			glm::vec3 u = y - x;
			glm::vec3 v = z - x;

		

			glm::vec3 result1 =  glm::normalize(glm::vec3((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), abs(u.x * v.y) - (u.y * v.x)));
			//result1 = glm::vec3(0.0f);
			//second triangle

			x = glm::vec3(0.5f*dimX, newTopRight, 0.5f*dimZ);
			z = glm::vec3(-0.5f*dimX, newBottomRight, 0.5f*dimZ);
			y = glm::vec3(-0.5f*dimX, newBottomLeft, -0.5f*dimZ);

			u = y - x;
			v = z - x;

			glm::vec3 result2 = glm::normalize(glm::vec3((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), abs(u.x * v.y) - (u.y * v.x)));
			//result2 = glm::vec3(0.0f);
			//glm::vec3 result1 = glm::vec3(0, 0, 0);
			//float newTopRight = (angle+)
			
			//std::cout << heightMap->getData(i, j) << " ";
			//creating map box
			float floorVertices[] = {
				// size										// texture coords			//normals		
				/*x*/	-0.5f*dimX,		newBottomLeft,	-0.5f*dimZ,			0.0f, 0.0f,				result1.x, result1.y,result1.z,	//first		//LEFT DOWN
				/*y*/	 0.5f*dimX,		newTopLeft,		-0.5f*dimZ,			0.0f, 1.0f,				result1.x, result1.y,result1.z,
				/*z*/	 0.5f*dimX,		newTopRight,	 0.5f*dimZ,			1.0f, 1.0f,				result1.x, result1.y,result1.z,

				/*x2*/	 0.5f*dimX,		newTopRight,	 0.5f*dimZ,			1.0f, 1.0f,				result2.x, result2.y,result2.z, //second
				/*y2*/	-0.5f*dimX,		newBottomRight,  0.5f*dimZ,			1.0f, 0.0f,				result2.x, result2.y,result2.z,
				/*z2*/	-0.5f*dimX,		newBottomLeft,  -0.5f*dimZ,			0.0f, 0.0f,				result2.x, result2.y,result2.z,

				/*-0.5f*dimX +dimX, angle, 0.5f*dimZ,				0.0f, 0.0f,				0.0f, 1.0f, 0.0f,
				-0.5f*dimX + dimX, newLeft, -0.5f*dimZ,			0.0f, 1.0f,				0.0f, 1.0f, 0.0f,	//first			//LEFT UP
				0.5f*dimX + dimX, newTopLeft, -0.5f*dimZ,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f,
				0.5f*dimX + dimX, newTopLeft, -0.5f*dimZ,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f,//second
				0.5f*dimX + dimX, newTop, 0.5f*dimZ,			1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
				-0.5f*dimX + dimX, angle, 0.5f*dimZ,		0.0f, 0.0f,				0.0f, 1.0f, 0.0f,

				-0.5f*dimX, newBottomRight, 0.5f*dimZ + dimX,				0.0f, 0.0f,				0.0f, 1.0f, 0.0f,		//RIGHT DOWN
				-0.5f*dimX, newBottom, -0.5f*dimZ + dimX,			0.0f, 1.0f,				0.0f, 1.0f, 0.0f,	//first
				0.5f*dimX, angle, -0.5f *dimZ + dimX,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f,
				0.5f*dimX, angle, -0.5f*dimZ + dimX,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f,//second
				0.5f*dimX, newRight, 0.5f *dimZ + dimX,			1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
				-0.5f*dimX, newBottomRight, 0.5f*dimZ + dimX,		0.0f, 0.0f,				0.0f, 1.0f, 0.0f,

				-0.5f*dimX + dimX, angle, -0.5f*dimZ + dimX,			0.0f, 0.0f,				0.0f, 1.0f, 0.0f,	//floor //	RIGHT UP
				0.5f*dimX + dimX, newTop, -0.5f*dimZ + dimX,			0.0f, 1.0f,				0.0f, 1.0f, 0.0f,
				0.5f*dimX + dimX, newTopRight, 0.5f*dimZ + dimX,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f, //first
				0.5f *dimX + dimX,  newTopRight, 0.5f *dimZ + dimX,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f, //second
				-0.5f*dimX + dimX,  newRight, 0.5f *dimZ + dimX,			1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
				-0.5f *dimX + dimX,  angle, -0.5f*dimZ + dimX,		0.0f, 0.0f,				0.0f, 1.0f, 0.0f,
				/*
				-0.5f, -0.5f*angle3, -0.5f,			0.0f, 0.0f,				0.0f, 1.0f, 0.0f,	//floor
				0.5f, -0.5f, -0.5f,			0.0f, 1.0f,				0.0f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.5f,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.5f,			1.0f, 1.0f,				0.0f, 1.0f, 0.0f,
				-0.5f, -0.5f*angle3, 0.5f,			1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
				-0.5f, -0.5f*angle3, -0.5f,		0.0f, 0.0f,				0.0f, 1.0f, 0.0f*/
			};

			unsigned int floorVAO, floorVBO, floorEBO;
			glGenVertexArrays(1, &floorVAO);
			glGenBuffers(1, &floorVBO);
			glGenBuffers(1, &floorEBO);

			glBindVertexArray(floorVAO);
			glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			//normals
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(2);

			VAO[counter++] = floorVAO;
		}
		//std::cout << "\n";
	}

}


void Map::render(unsigned int chestVAO, unsigned int spikeVAO, Shader ourShader, HeightMap* heightMap)
{
	ourShader.setInt("totalTorches", torchCounter);
	if (iter > colorTime) {
		wierdColor = glm::vec3((float(248) / 255), (float(240) / 255), (float(10) / 255));
		if (iter > colorTime*2) {
			iter = 0;
			
			wierdColor = glm::vec3(0, 0, 1);
		}
	}
	else {
		//wierdColor = glm::vec3((float(10) / 255), (float(100) / 255), (float(10) / 255));
	}
	float placeX = 0.0f;
	float placeZ = 0.0f;
	int counter = 0;
	for (int i = 0; i < heightMap->getHeight(); i++) {
		placeX = 0;
		for (int j = 0; j < heightMap->getWidth(); j++) {
			placeX++;
			//float currentHeight = heightMap->getData(i, j);
			glBindVertexArray(VAO[counter++]);
			glm::mat4 model = glm::mat4(0.5f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3((i * dimX), 0, (j * dimX)));
			

			float angle = heightMap->getData(i, j);

			//color testing
			glm::vec3 green = glm::vec3((float(101) / 255), (float(67) / 255), (float(30) / 255));
			glm::vec3 brown = glm::vec3((float(10) / 255), (float(100) / 255), (float(10) / 255));

			float colorVal = angle;

			glm::vec3 currentColor = glm::vec3(brown.x + ((green.x - brown.x)*colorVal), brown.y + ((green.y - brown.y)*colorVal), brown.z + ((green.z - brown.z)*colorVal));

			if (colorVal == 0 || rememberBlue[counter]) {
				currentColor = glm::vec3(0, 0, 1);
			}
			
			if (beachVal[counter] == true) {
				currentColor = beachColor;
			}
			else if (rememberVal[counter] == true) {
				
				currentColor = wierdColor;
			}
			else if (snowVal[counter]) {
				currentColor = snowColor;
			}

			if (walkedVal[counter]) {
				currentColor = glm::vec3(0,0,0);
			}

			
			//float angle = 20.0f * 0;
			ourShader.setVec3("ambientColor", currentColor);
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			/*
			model = glm::translate(model, glm::vec3(1, 0, 0));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			model = glm::translate(model, glm::vec3(0, 0, 1));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			model = glm::translate(model, glm::vec3(-1, 0, 0));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);*/
		}
		
		placeZ++;
	}

	glm::vec3 brown = glm::vec3((float(101) / 255), (float(67) / 255), (float(30) / 255));
	glm::vec3 green = glm::vec3((float(10) / 255), (float(100) / 255), (float(10) / 255));

	float colorVal = 1.0f;

	glm::vec3 currentColor = glm::vec3(brown.x +((green.x - brown.x)*colorVal), brown.y + ((green.y - brown.y)*colorVal), brown.z + ((green.z - brown.z)*colorVal));

	ourShader.setInt("totalTorches", torchCounter);
	



	/*//adding torches
	for (int i = 0; i < torchCounter; i++)
	{
		
		std::string nr;
		std::stringstream ss;
		ss << i;
		nr = ss.str();
		glm::vec3 torchOff = glm::vec3(0, 0, 0);
		if (torchType[i] == 2)
		{
			torchOff = glm::vec3(0.0f, 0.1f, 0.1f);
		}
		else if (torchType[i] == 3)
		{
			torchOff = glm::vec3(0.1f, 0.1f, 0.0f);
		}
		else if (torchType[i] == 4)
		{
			torchOff = glm::vec3(0.0f, 0.1f, -0.1f);
		}
		else if (torchType[i] == 5)
		{
			torchOff = glm::vec3(-0.1f, 0.1f, 0.0f);
		}

		ourShader.setVec3("lightPos[" + nr + "]", (torchPositions[i] + torchOff) * glm::vec3(0.5f, 0.5f, 0.5f));
		//std::cout << torchPositions[i].x << " AND "<<nr<<" " << torchPositions[i].z << std::endl;
	}*/
	ourShader.setInt("totalTorches", torchCounter);
	//std::cout << torchCounter;
	
	//adding torches
	for (int i = 0; i < torchCounter; i++)
	{

		std::string nr;
		std::stringstream ss;
		ss << i;
		nr = ss.str();
		glm::vec3 torchOff = glm::vec3(0, 0, 0);
		if (torchType[i] == 2)
		{
			torchOff = glm::vec3(0.0f, 0.1f, 0.1f);
		}
		else if (torchType[i] == 3)
		{
			torchOff = glm::vec3(0.1f, 0.1f, 0.0f);
		}
		else if (torchType[i] == 4)
		{
			torchOff = glm::vec3(0.0f, 0.1f, -0.1f);
		}
		else if (torchType[i] == 5)
		{
			torchOff = glm::vec3(-0.1f, 0.1f, 0.0f);
		}

		ourShader.setVec3("lightPos[" + nr + "]", (torchPositions[i] + torchOff) * glm::vec3(0.5f, 0.5f, 0.5f));
		//std::cout << torchPositions[i].x << " AND "<<nr<<" " << torchPositions[i].z << std::endl;
	}

	//DRAW SKY
	if (cloudActive) {
		glBindVertexArray(VAO1);
		for (int i = 0; i < cloudCounter; i++) {
			//draw cloud
			glm::mat4 model2 = glm::mat4(0.5f); // make sure to initialize matrix to identity matrix first
			glm::vec3 temp2 = glm::vec3(cloudPositions[i].x, cloudPositions[i].y, cloudPositions[i].z);
			model2 = glm::translate(model2, temp2);
			model2 = glm::scale(model2, cloudSize[i]);
			ourShader.setVec3("ambientColor", glm::vec3(0.8, 0.8, 0.8));
			ourShader.setMat4("model", model2);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	if(rainActive){
		glBindVertexArray(VAO1);
		for (int i = 0; i < cubeCounter; i++)
		{
			
			glm::mat4 model = glm::mat4(0.5f); // make sure to initialize matrix to identity matrix first
			glm::vec3 temp = glm::vec3(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
			model = glm::translate(model, temp);
			model = glm::scale(model, glm::vec3(0.005f, 0.01f, 0.005f));

			ourShader.setVec3("ambientColor", glm::vec3(0,0,1));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//std::cout << "RAIN DROOOP IS " << cubePositions[i].y << " YOOOOO\n\n\n";

		}
		updateRaindrops();
	}

	//change texture
	//texture2.bind();
	//ourShader.setInt("texture1", 1);
	glBindTexture(GL_TEXTURE_2D, chestVAO);

	if (sunActive) {
		ourShader.setBool("sunActive", true);
		sunAngle = 0.0f;
		//drawing sun
		glBindVertexArray(VAO1);
		ourShader.setVec3("ambientColor", glm::vec3(1, 1, 0));
		ourShader.setBool("sunColor", true);
		for (int i = 0; i < 360; i++) {
			for (int j = 0; j < 3; j++) {
				glm::mat4 model2 = glm::mat4(0.5f); // make sure to initialize matrix to identity matrix first
				model2 = glm::translate(model2, sunPosition);
				model2 = glm::scale(model2, glm::vec3(0.5, 0.5, 0.5));
				if (j == 0)
					model2 = glm::rotate(model2, glm::radians(sunAngle), glm::vec3(1, 0, 0));
				if (j == 1)
					model2 = glm::rotate(model2, glm::radians(sunAngle), glm::vec3(0, 1, 0));
				if (j == 2)
					model2 = glm::rotate(model2, glm::radians(sunAngle), glm::vec3(0, 0, 1));

				//model2 = glm::scale(model2, glm::vec3(10.0f, 10.0f, 10.0f));

				ourShader.setMat4("model", model2);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			sunAngle += 1.0f;
		}
		ourShader.setBool("sunColor", false);
		moveSun();
	}
	else {
		ourShader.setBool("sunActive", false);
	}

	glBindTexture(GL_TEXTURE_2D, spikeVAO);
	glBindVertexArray(VAO1);
	//render spikes
	for (int i = 0; i < spikeCounter; i++) {
		glm::mat4 model9 = glm::mat4(0.5f); // make sure to initialize matrix to identity matrix first
		model9 = glm::translate(model9, spikePositions[i]);
		model9 = glm::scale(model9, glm::vec3(0.25f, 0.5f, 0.25f));
		//float angle = 20.0f * 0;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		ourShader.setMat4("model", model9);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}

void Map::initMapfiles() {
	
}

void Map::initializeWall(Shader ourShader)
{
	//RAIN DROP

	float wallVertices[] = {
		// size							 // texture coords					//normals			
		-0.5f, -0.5f, -0.5f,			0.0f, 0.0f,							0.0f,  0.0f, -1.0f,	//x-side
		 0.5f, -0.5f, -0.5f,			0.0f, 1.0f,							0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,			1.0f, 1.0f,							0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,			1.0f, 1.0f,							0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,			1.0f, 0.0f,							0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,			0.0f, 0.0f,							0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,			0.0f, 0.0f,							0.0f,  0.0f, 1.0f,	//x-side2
		 0.5f, -0.5f,  0.5f,			0.0f, 1.0f,							0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,			1.0f, 1.0f,							0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,			1.0f, 1.0f,							0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,			1.0f, 0.0f,							0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,			0.0f, 0.0f,							0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,			0.0f, 0.0f,							-1.0f,  0.0f,  0.0f,	//z-side
		-0.5f,  0.5f, -0.5f,			0.0f, 1.0f,							-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,			1.0f, 1.0f,							-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,			1.0f, 1.0f,							-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,			1.0f, 0.0f,							-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,			0.0f, 0.0f,							-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,			0.0f, 0.0f,							1.0f,  0.0f,  0.0f, //z-side2
		 0.5f,  0.5f, -0.5f,			0.0f, 1.0f,							1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,			1.0f, 1.0f,							1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,			1.0f, 1.0f,							1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,			1.0f, 0.0f,							1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,			0.0f, 0.0f,							1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,			0.0f, 0.0f,							0.0f, -1.0f,  0.0f,	//floor
		 0.5f, -0.5f, -0.5f,			0.0f, 1.0f,							0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,			1.0f, 1.0f,							0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,			1.0f, 1.0f,							0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,			1.0f, 0.0f,							0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,			0.0f, 0.0f,							0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,			0.0f, 0.0f,							0.0f, 1.0f,  0.0f,	//ceiling
		 0.5f,  0.5f, -0.5f,			0.0f, 1.0f,							0.0f, 1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,			1.0f, 1.0f,							0.0f, 1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,			1.0f, 1.0f,							0.0f, 1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,			1.0f, 0.0f,							0.0f, 1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,			0.0f, 0.0f,							0.0f, 1.0f,  0.0f

	};

	unsigned int floorVAO1, floorVBO1, floorEBO1;
	glGenVertexArrays(1, &floorVAO1);
	glGenBuffers(1, &floorVBO1);
	glGenBuffers(1, &floorEBO1);

	glBindVertexArray(floorVAO1);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	VAO1 = floorVAO1;
}

//converts a vector from absolute scene coordinates to coordinates on our tile based map
glm::vec3 Map::convertToGridCoordinates(glm::vec3 input)
{

	int x = int(input.x + float(mapLengthX) / 2);
	int z = int(input.z + float(mapLengthZ) / 2);
	glm::vec3 output = glm::vec3(x, 0.0f, z);
	return output;

}

//convert back from map coordinates to coordinates in the openGL scene
glm::vec3 Map::convertToSceneCoordinates(glm::vec3 input)
{
	const float offset = 0.5f; //offset to make the recalculated value the center of the tile

	float x = (input.x - (mapLengthX / 4) - TILE_SIZE - bonusX);
	float z = (input.z - (mapLengthZ / 4) - TILE_SIZE - bonusZ);
	glm::vec3 output = glm::vec3(x + offset, -0.5f, z + offset);

	float x2 = (input.x + float(mapLengthX) / 2);
	float z2 = (input.z + float(mapLengthZ) / 2);
	glm::vec3 output2 = glm::vec3(x, -0.5f, z);
	return output2;
}

void Map::drawTorches(Shader objectShader, int torchId) {
	
	glm::vec3 current = getTorchPos(torchId);
	//std::cout << "TOTAL=" << tot << std::endl << std::endl;
	glm::mat4 model4 = glm::mat4(1.0f);
	glm::vec3 torchDist = glm::vec3(0.0f, 0.0f, 0.0f);

	float torchAngle = 0.0f;
	float torchOffset = 0.133f;
	if (getTorchType(torchId) == 2) {
		torchDist.x -= torchOffset;
		torchAngle = 90.0f;
	}
	else if (getTorchType(torchId) == 3) {
		torchDist.z += torchOffset;
		torchAngle = 180.0f;
	}
	else if (getTorchType(torchId) == 4) {
		torchDist.x += torchOffset;
		torchAngle = 270.0f;
	}
	else if (getTorchType(torchId) == 5) {
		torchDist.z -= torchOffset;

	}
	model4 = glm::translate(model4, current + torchDist);
	model4 = glm::scale(model4, glm::vec3(0.033f, 0.033f, 0.033f));
	model4 = glm::rotate(model4, glm::radians(torchAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	objectShader.setMat4("model", model4);
}

//checks if there is a wall at the given absolute position
bool Map::notWall(float x, float z)
{
	//if(noclip){return true;}
	
	//convert the given float values from absolute coordinates to a grid reference
	glm::vec3 convertedPos = convertToGridCoordinates(glm::vec3(x, 0.0f, z));
	//std::cout << convertedPos.x << " POSSS " << convertedPos.z << std::endl;
	
	//goes through all walls, checks if the given coordinates are a wall.
	for (int i = 0; i < cubeCounter; i++)
	{
		
		glm::vec3 convertedPos2 = convertToGridCoordinates(glm::vec3(cubePositions[i].x, 0.0f, cubePositions[i].z));
		//std::cout << cubePositions[i].x << " " << cubePositions[i].z << std::endl;
		if (convertedPos == convertedPos2) {
			return false;
		}
		//std::cout <<int(cubePositions[i].x + float(mapLengthX) / 2) << " " << int(cubePositions[i].z + float(mapLengthZ) / 2)  << std::endl;
		//if(cubePositions[i].x +  == convertedPos.x && cubePositions[i].z == convertedPos.z)
			//return false;
	}
	//std::cout << "\n\n\n";
	//std::cout << convertedPos.x << " " << convertedPos.z << std::endl;
	return true;	//if there is not, return true.
}


//check for all keys if any have been found
void Map::checkFoundKeys(glm::vec3 playerPos)
{
	for (int i = 0; i < chestCounter; i++)
	{
		keys[i]->checkFound(playerPos);
	}
}

//checks if we've won the game. Returns a bool, which is whether that is the case or not.
bool Map::winGame()
{
	//go through all keys
	for (int i = 0; i < chestCounter; i++)
	{
		//if a key isn't found
		if (!keys[i]->isFound())
		{
			return false; //then we haven't won yet
		}
	}

	//if we're here then all keys were found, and we won! Yay!
	return true;
}
void Map::foundKey() {
	currentAmountKey++;
}

void Map::updateRaindrops() {
	
	int counter = 0;
	for (int j = 0; j < cloudCounter; j++) {
		for (int i = 0; i < cloudAmount[j]; i++) {
			
			cubePositions[counter].y -= raindropSpeed;
			if (cubePositions[counter].y < rainMin) {
				//set new position from top

				float randX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float randZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				randX *= cloudSize[j].x;
				randZ *= cloudSize[j].z;
				cubePositions[counter] = glm::vec3(float(randX)+cloudPositions[j].x-(0.5*cloudSize[j].x), cloudPositions[j].y, float(randZ)+cloudPositions[j].z- (0.5*cloudSize[j].z));
				
			}
			counter++;
		}
	}
}

void Map::createRaindrops() {
	
	cubeCounter = 0;
	for (int i = 0; i < cloudCounter; i++) {
		int amountDrops = int(cloudSize[i].x*cloudSize[i].z*rainDensity);//calc later
		cloudAmount[i] = amountDrops;																 //std::cout<<"cloud is " << cloudSize[i].x << " "<< cloudSize[i].z << " and amount is " << amountDrops << "rdrop\n";
		for (int j = 0; j < amountDrops; j++) {

			//int randX = (rand() % int(10*cloudSize[i].x)) + 1;
			//int randZ = (rand() % int(10 * cloudSize[i].z)) + 1;
			float randX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float randZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			randX *= cloudSize[i].x;
			randZ *= cloudSize[i].z;
			float randY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			cubePositions[cubeCounter++] = glm::vec3(float(randX) + cloudPositions[i].x - (0.5f*cloudSize[i].x), randY*cloudPositions[i].y, float(randZ) + cloudPositions[i].z - (0.5f*cloudSize[i].z));
			//std::cout << cubePositions[cubeCounter - 1].y << " and " << j << "\n";
		}
	}
}

void Map::moveSun() {
	
	setTorchPos(1, sunPosition);
	//update velocity
	//std::cout << sunPosition.y << "\n";
	if (sunPosition.y < 0.0f) {
		//std::cout << "RESET SUN\n";
		//reset sun position
		sunPosition.y = sunStartPosition.y;
		sunPosition.x = sunStartPosition.x;
		sunPosition.z = sunStartPosition.z;
		sunVel.y = sunStartVel.y;
	}

		sunPosition.y += sunVel.y *deltaTime;
		sunPosition.x += sunVel.x * deltaTime;
		sunPosition.z += sunVel.z * deltaTime;
		sunVel.y -= gravity * deltaTime;

}

glm::vec3 Map::getSpikePos(int nr) {
	return spikePositions[nr];
}
int Map::getSpikeCount() {
	return spikeCounter;
}

int Map::getTotalKeys() {
	return chestCounter;
}

int Map::getAmountKeys() {
	return currentAmountKey;
}


//getters

int Map::getCubeCounter()
{
	return cubeCounter;
}

int Map::getCurrentX()
{
	return currentX;
}

int Map::getCurrentZ()
{
	return currentZ;
}

int Map::getMapLengthX()
{
	return mapLengthX;
}

int Map::getMapLengthZ()
{
	return mapLengthZ;
}

glm::ivec2 Map::getMapSize()
{
	return glm::ivec2(mapLengthX, mapLengthZ);
}

float Map::getBonusX()
{
	return bonusX;
}

glm::vec3 Map::getSunPos() {
	return sunPosition;
}

float Map::getBonusZ()
{
	return bonusZ;
}

glm::vec3 Map::getTorchPos(int torchId)
{
	return torchPositions[torchId];
}

int Map::getTorchCount()
{
	return torchCounter;
}

int Map::getTorchType(int torchId)
{
	return torchType[torchId];
}

void Map::makeLight(glm::vec3 pos) {
	
	torchType[torchCounter] = 0;
	torchPositions[torchCounter++] = pos;
	
}

void Map::setTorchPos(int i, glm::vec3 pos) {
	torchPositions[i] = pos;
}


void Map::toggleRain(bool val) {
	rainActive = val;
}
void Map::setRainDensity(float density) {
	if (density != rainDensity) {
		rainDensity = density;
		createRaindrops();
	}
}
float Map::setRainDensity() {
	return rainDensity;
}
float Map::getYPos(float xPos, float zPos) {
	int num = hm->getNum(xPos, zPos);
	walkedVal[num] = true;
	//std::cout << hm->getData((int)xPos*hm->getHeight(), (int)zPos*hm->getWidth()) << "WIDTH\n";
	//std::cout << int(float(xPos/hm->getWidth())*20*hm->getWidth()) << " and " << int(float(zPos / hm->getHeight()) * 20 * hm->getHeight()) << "\n";
	//std::cout << hm->getData(int(xPos*(float)hm->getWidth()), int(zPos*(float)hm->getHeight())) << " ";
	return hm->getData(int(float(xPos / hm->getWidth()) * (1/dimX) * hm->getWidth()), int(float(zPos / hm->getHeight()) * (1/dimZ) * hm->getHeight()));
}

bool Map::getSunActive() {
	return sunActive;
}
void Map::setSunActive(bool val) {
	sunActive = val;
}
void Map::setCloudActive(bool val) {
	cloudActive = val;
}

float Map::getDimX() {
	return dimX;
}
float Map::getDimZ() {
	return dimZ;
}


void Map::makeCloud(glm::vec3 pos, glm::vec3 cSize) {
	cloudSize[cloudCounter] = cSize;
	cloudPositions[cloudCounter++] = pos;
	createRaindrops();
	
}

float Map::mapping(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//destructor
Map::~Map()
{
	delete[] keys;
}