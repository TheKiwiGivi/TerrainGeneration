#include <GL/glew.h>
#include "Camera.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

//own files
#include "input.h"
#include "mouse.h"
#include "enemy.h"
#include "map.h"
#include "ImageTexture.h"
#include "Shader.h"
#include "Model.h"
#include "player.h"
#include "heightmap.h"

#include <time.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// IMGUI INCLUDES
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

// Sound
#include "klang/irrKlang.h"

#include "Skybox.h"
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera playerCam(glm::vec3(0.0f, 0.0f, 3.0f));

//camera view
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//void framebuffer_resize_callback(GLFWwindow* window, int& fbwidth, int& fbheight);
//void scroll_callback2(GLFWwindow* window, double xoffset, double yoffset, Player* player);

//imgui menu
HeightMap* gameMenu(GLFWwindow* window, Shader textureShader);

//finding map files
void initMapFiles();
bool useCursor = false;


int hmSize = 100;
int hmHeight = 100;


int enemyCounter = 0;
int enemyID = 0;
//enemy starting position and rotation
glm::vec3 startPos = glm::vec3(0.5f, -0.5f, 0.5f);
glm::vec3 enemyPositions[MAX_ENEMIES];
Enemy* enemies[MAX_ENEMIES];

std::string selectedPath = "resources/map/";

// settings
int cameraMode = 0;
float fov = 45.0f;

float iter = 0.0f;

double xpos = 0;
double ypos = 0;

bool moveFree = true;

//game logic
bool isDead = false;
bool wonTheGame = false;
int maps[100];
int totalMaps = 0;
int selectedMap = 0;
int loadMap = 0;

int selectedBiome = 0;

//from menu to game code
void startGame(GLFWwindow* window);

//check if player died
bool checkDead(Map* map, GLFWwindow* window, Player* player);


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int amountDeer = 1;



GLFWwindow* createWindow(const char* title, const int width, const int height, int& fbWidth, int& fbHeight, int GLmajorVer, int GLminorVer, bool resizable)
{
	// glfw: initialize and configure

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLmajorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLminorVer);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	return window;
}

HeightMap* loadImg(const char* filePath) {
	const char* k = "k";
	//unsigned char* data = stbi_load(k, &width, &height, &nrChannels, 0);


	HeightMap* heightMap = new HeightMap(filePath);
	return heightMap;
}

HeightMap* createImg(int biome, int hmSize) {
	const char* k = "k";
	//unsigned char* data = stbi_load(k, &width, &height, &nrChannels, 0);


	HeightMap* heightMap = new HeightMap(biome, hmSize);
	return heightMap;
}

int main()
{
	// Start sound
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	SoundEngine->play2D("audio/spill.wav", GL_TRUE);

	int fbWidth = SCR_WIDTH;
	int fbHeight = SCR_HEIGHT;

	glfwInit();

	GLFWwindow* window = createWindow("Terrain Generation", SCR_WIDTH, SCR_HEIGHT, fbWidth, fbHeight, 3, 3, false);
	// tell GLFW to capture our mouse

	

	// init GLEW
	glewExperimental = true;	// improve support for newer OpenGL releases
	glewInit();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	//create player
	Player* player = new Player(&playerCam);

	// build and compile our shader zprogram
	Shader textureShader("texture.vs", "texture.fs");
	Shader objectShader("object.vs", "object.fs");

	Shader simpleDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.fs");

	

	ImageTexture tex("resources/textures/awesomeface.png", png);



	//shadow map test
	//MAYBE UNSIGNED INT WRONG NR

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// shader configuration
	// --------------------
	textureShader.use();
	textureShader.setInt("diffuseTexture", 0);
	textureShader.setInt("shadowMap", 1);


	//std::cout << tex.getID() << " IS ID\n\n\n";




	Model deerModel("resources/model/deer.obj");
	Model torchModel("resources/torch/Torch.obj");

	//random
	srand(time(NULL));


	// load and create a texture 
	//chest
	//ImageTexture texture1("resources/textures/container2.png", png);
	//map
	ImageTexture texture2("resources/textures/awesomeface.png", png);
	//spikes
	//ImageTexture texture3("resources/textures/bricks2_disp.jpg", jpg);










	// IMGUI
	const char* glsl_version = "#version 330";
	// Make imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	initMapFiles();
	// Make imgui game menu
	HeightMap* heightMap = gameMenu(window, textureShader);


	
	//init map
	Map* map = new Map(textureShader, heightMap);

	map->makeLight(player->getPos());
	//map->makeLight(map->getSunPos());

	//make deer
	//Enemy deer(map, glm::vec3(0, 0, 0));

	// make skybox
	Skybox* skybox = new Skybox();



	for (int i = 0; i < amountDeer; i++) {
		enemyPositions[enemyCounter++] = glm::vec3(((float)rand() / (float)RAND_MAX)*hmSize/20, 0.0f, ((float)rand() / (float)RAND_MAX)*hmSize/20);
	}
	//initialize enemies
	for (int i = 0; i < enemyCounter; i++) {
		
		enemies[i] = new Enemy(map, enemyPositions[i]);
		startPos.x++;
	
	}


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		
		//time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glEnable(GL_CLIP_DISTANCE0);

		iter += deltaTime;




		//trying shadow render

		/*
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); 
		// note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene


		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(map->getSunPos(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, woodTexture);
		texture1.bind();
		

		//CALL DRAW HERE!!!



		//reset ?
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		textureShader.use();
		glm::mat4 projection3 = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view3 = glm::lookAt(player->getPos(), player->getPos() + player->getCamFront(), player->getCamUp());


		textureShader.setMat4("projection", projection3);
		textureShader.setMat4("view", view3);
		// set light uniforms
		//player pos wrong maybe?
		textureShader.setVec3("viewPos", player->getPos());

		//maybe
		textureShader.setVec3("lightPos", map->getSunPos());


		textureShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);*/

		//maybe not
		/*
		glActiveTexture(GL_TEXTURE0);
		texture1.bind();
		glActiveTexture(GL_TEXTURE1);
		*/

		//maybe not
		//glBindTexture(GL_TEXTURE_2D, depthMap);

		/*

		//render map!
		textureShader.setVec3("viewPos", player->getPos());
		map->render(texture1.getID(), texture3.getID(), textureShader, heightMap);
		*/
		

		//CALL DRAW HERE!!!!!!!!!!!
		
		//textureShader.use()

		//map->render(texture1.getID(), texture3.getID(), textureShader, heightMap);



		// input
		if (!isDead && !wonTheGame) {
			processInput(window, map, player);
		}

		// make imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//mouse
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_callback2(window, xpos, ypos, player);

		if (!player->isJumping()) {
			isDead = checkDead(map, window, player);
		}
		GLfloat vert[] = {
			20, 0, 0,
			10, 10, 0,
			50, 20, 0,
			50, 30, 0
		};

		
		//try heightmap Yoooo
		//glBegin(GL_TRIANGLE_STRIP);
		//glVertex3f();
		//glEnd();

		//player info (keys, fps)
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::SetNextWindowPos(ImVec2(0, 0));

			ImGui::Begin("Info", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);

			//cursor logic
			if (useCursor) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Pos: %f %f %f", player->getPos().x, player->getPos().y, player->getPos().z);
			ImGui::End();
			
			ImGui::SetNextWindowPos(ImVec2(0, 50));
			ImGui::SetNextWindowSize(ImVec2(250, 250));

			ImGui::Begin("Lighting", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);

			static float i0 = 0.0f;
			static float i1 = 0.0f;
			static float i2 = 0.0f;


			static float myAtt = 1.0f;
			if (ImGui::TreeNode("Lighting"))
			{
				

				ImGui::SliderFloat("Str", &myAtt, 0.01, 1.0f);
				player->setAttenuation(myAtt);


			ImGui::InputFloat("x", &i0);
			ImGui::InputFloat("y", &i1);
			ImGui::InputFloat("z", &i2);
			if (ImGui::Button("Make light on specified")) {
				glm::vec3 tempy = glm::vec3(i0, i1, i2);
				map->makeLight(tempy);
			}
		

			if (ImGui::Button("Make light on me")) {
				map->makeLight(player->getPos());
			}
			ImGui::TreePop();
			}
			static float j1 = 0;
			static float j2 = 0;
			static float j3 = 0;
			if (ImGui::TreeNode("Camera"))
			{

				
				

				j1 = player->getPos().x;
				j2 = player->getPos().y;
				j3 = player->getPos().z;

				ImGui::SliderFloat("Cam x", &j1, 0, 5.0f);
				ImGui::SliderFloat("Cam y", &j2, 0, 3);
				ImGui::SliderFloat("Cam z", &j3, 0, 5.0f);
				
					player->setXPos(j1);
					player->setYPos(j2);
					player->setZPos(j3);
				

				
				ImGui::TreePop();
			}
			//ImGui::SameLine();
			static float density = 500.0f;
			if (ImGui::TreeNode("Weather")){
			
			ImGui::SliderFloat("Rain density", &density, 0, 1000);
			map->setRainDensity(density);



			//make cloud
			static float x0 = 0.0f;
			static float x1 = 0.0f;
			static float x2 = 0.0f;

			static float y0 = 0.25f;
			static float y2 = 0.25f;


			ImGui::InputFloat("x", &x0);
			ImGui::InputFloat("y", &x1);
			ImGui::InputFloat("z", &x2);

			

			ImGui::SliderFloat("Size x", &y0, 0.25, 1/map->getDimX());
			ImGui::SliderFloat("Size z", &y2, 0.25, (heightMap->getHeight()) / 20);


			if (ImGui::Button("Make cloud on specified")) {
				glm::vec3 tempy = glm::vec3(x0, x1, x2);
				map->makeCloud(tempy, glm::vec3(y0, 0.01f, y2));
			}


			if (ImGui::Button("Make cloud on me")) {
				map->makeCloud(player->getPos(), glm::vec3(y0, 0.01f, y2));
			}

			ImGui::TreePop();
			}


			static bool rain = true;
			static bool sun = true;
			static bool cloud = true;
			static bool walk = false;
			static bool attOn = false;
			ImGui::Checkbox("Cloud active", &cloud);
			ImGui::Checkbox("Rain active", &rain);
			ImGui::Checkbox("Sun active", &sun);
			ImGui::Checkbox("Walk mode", &walk);
			ImGui::Checkbox("Light on me", &attOn);
			map->toggleRain(rain);
			map->setSunActive(sun);
			map->setCloudActive(cloud);
			player->setWalking(walk);
			player->setAttenuationOn(attOn);
			

			ImGui::End();

			if (map->getTotalKeys() <= map->getAmountKeys()) {
				//wonTheGame = true;
			}
			if (isDead) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				ImGui::SetNextWindowSize(ImVec2(250, 250));
				ImGui::SetNextWindowPos(ImVec2((SCR_WIDTH / 2) - 250 / 2, (SCR_HEIGHT / 2) - 250 / 2));
				ImGui::Begin("Dead.com", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoTitleBar);

				ImGui::Text("You died! What now?");
				if (ImGui::Button("Respawn")) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					//RESET POS
					player->setXPos(0.0f);
					player->setZPos(0.0f);
					isDead = false;
				}
				ImGui::End();
			}
			else if (wonTheGame) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				ImGui::SetNextWindowSize(ImVec2(250, 250));
				ImGui::SetNextWindowPos(ImVec2((SCR_WIDTH / 2) - 250 / 2, (SCR_HEIGHT / 2) - 250 / 2));
				ImGui::Begin("won.com", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoTitleBar);
				//ImGui::Begin("Key Labyrinth!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("You Won! You're awesome :)");               // Display some text (you can use a format strings too)
				if (ImGui::Button("Exit")) {
					return 0;
				}
				ImGui::End();
			}
		}

		// Render imgui
		ImGui::Render();
		int display_w, display_h;

		// render

		texture2.bind();

		// activate shader
		textureShader.use();

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		map->setTorchPos(1, player->getPos());
		map->setTorchPos(0, map->getSunPos());
		//std::cout << "pos: " << map->getTorchPos(0).x << " " << map->getTorchPos(0).y << " " << map->getTorchPos(0).z << "\n";
		texture2.bind();
		//texture2.bind();

		// activate shader
		textureShader.use();

		// pass projection matrix to shader 
		//used for camera mode
		glm::vec3 offset = glm::vec3(0.0, 0.0, 0.0);
		float farPlane = 100.0f;
		float nearPlane = 0.1f;
		if (cameraMode == 1) {
			//std::cout << "WORKS";
			offset.x = -0.0f;
			offset.y = 1.0;
			
			
		}

		/*glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);
		textureShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(player->getPos() + offset, player->getPos() + player->getCamFront(), player->getCamUp());
		textureShader.setMat4("view", view);*/

		glm::vec3 lightPos = glm::vec3(0, 0, 0);











		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); 
		// note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene


		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(map->getSunPos(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, tex.getID());
		//texture2.bind();
		simpleDepthShader.use();

		//CALL DRAW HERE!!!
		//map->render(texture1.getID(), texture3.getID(), simpleDepthShader, heightMap);
		//map->render(texture2.getID(), tex.getID(), simpleDepthShader, heightMap);
		textureShader.use();
		//reset ?
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		textureShader.use();
		glm::mat4 projection2 = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view2 = glm::lookAt(player->getPos()+offset.y, player->getPos() + player->getCamFront(), player->getCamUp());


		textureShader.setMat4("projection", projection2);
		textureShader.setMat4("view", view2);
		// set light uniforms
		//player pos wrong maybe?
		textureShader.setVec3("viewPos", player->getPos());

		//maybe
		//textureShader.setVec3("lightPos[0]", map->getSunPos());


		textureShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//maybe not
		/*
		glActiveTexture(GL_TEXTURE0);
		texture1.bind();
		glActiveTexture(GL_TEXTURE1);
		*/

		//maybe not
		//glBindTexture(GL_TEXTURE_2D, depthMap);

		/*

		//render map!
		textureShader.setVec3("viewPos", player->getPos());
		map->render(texture1.getID(), texture3.getID(), textureShader, heightMap);
		*/

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, tex.getID());
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		

		//CALL DRAW HERE!!!!!!!!!!!

		textureShader.use();

		//map->render(texture1.getID(), texture3.getID(), textureShader, heightMap);

		textureShader.setFloat("playerAtt", player->getAttenuation());
		textureShader.setBool("playerAttOn", player->getAttenuationOn());
		map->render(texture2.getID(), tex.getID(), textureShader, heightMap);




		//render map!
		//textureShader.setVec3("viewPos", player->getPos());
		

		//render player!
		//drawing torches
		objectShader.use();




		for (int i = 0; i < map->getTorchCount(); i++) {
			
			std::string nr;
			std::stringstream ss;
			ss << i;
			nr = ss.str();

			objectShader.setVec3("lightPos[" + nr + "]", (map->getTorchPos(i)) * glm::vec3(0.5f, 0.5f, 0.5f));
		}

		//render player!
		objectShader.use();
		objectShader.setMat4("projection", projection2);
		objectShader.setMat4("view", view2);
		objectShader.setVec3("viewPos", player->getPos());
		objectShader.setInt("totalTorches", map->getTorchCount());

		player->render(objectShader);
		deerModel.Draw(objectShader);


		//enemies[0]->render(objectShader);
		//deerModel.Draw(objectShader);

		int tot = map->getTorchCount();
		for (int i = 0; i < tot; i++) {
			objectShader.use();
			map->drawTorches(objectShader, i);
			torchModel.Draw(objectShader);
		}


		
		//ENEMY TEST

		//activate enemies
		for (int i = 0; i < enemyCounter; i++)
		{
			enemies[i]->activate(player->getPos());
		}
		for (int i = 0; i < enemyCounter; i++) {
			//	enemies[i]->step();

			enemies[i]->render(objectShader);
			deerModel.Draw(objectShader);

		}


		/*glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vert);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
		glDisableClientState(GL_VERTEX_ARRAY);*/

		// Trying to draw skybox
		// draw scene as normal
		glm::mat4 modelSky = glm::mat4(1.0f);
		glm::mat4 viewSky = camera.GetViewMatrix();
		glm::mat4 projectionSky = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

		viewSky = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skybox->draw(viewSky, projectionSky);

		glDepthFunc(GL_LESS); // set depth function back to default

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
/*void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
if (fov >= 1.0f && fov <= 45.0f)
fov -= yoffset;
if (fov <= 1.0f)
fov = 1.0f;
if (fov >= 45.0f)
fov = 45.0f;
}*/

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

int input = 0;

HeightMap* gameMenu(GLFWwindow* window, Shader textureShader)
{
	while (input != 1)
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		if (!loadMap) {
			static float f = 0.0f;
			static int counter = 0;
			ImGui::SetNextWindowSize(ImVec2(250, 250));
			ImGui::SetNextWindowPos(ImVec2((SCR_WIDTH / 2) - 250 / 2, (SCR_HEIGHT / 2) - 250 / 2));
			ImGui::Begin("Terrain generation", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoTitleBar);
			
			ImGui::Text("Terrain builder");

			if (ImGui::Button("How to use")) {
				//display how to play
				loadMap = 2;
			}
			ImGui::NewLine();
			ImGui::Text("Currently selected map: map_%d", selectedMap);               // Display some text (you can use a format strings too)
			ImGui::SliderInt("Deer count", &amountDeer, 0, 10);

			if (ImGui::Button("Load terrain from PNG")) {
				startGame(window);
				//const char* path = selectedMap;
				//HeightMap* heightMap = loadImg(path);
				std::string o = selectedPath+"map_"+std::to_string(selectedMap)+".png";
				const char* na = o.c_str();
				//const char* p = na;
				HeightMap* heightMap = loadImg(na);
				
				ImGui::End();
				ImGui::EndFrame();
				return heightMap;
			}
			if (ImGui::Button("Select Map")) {
				//load select map options
				loadMap = 1;
			}
			//static char mapName[64] = ""; ImGui::InputText("default", mapName, 64);
			//if (ImGui::Button("Load")) {
				//selectedMap =  mapName;
			//}
			//ImGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));

			

			ImGui::NewLine();
			
			if (ImGui::Button("Generate terrain")) {
				startGame(window);
				//const char* path = selectedMap;
				//HeightMap* heightMap = loadImg(path);
				HeightMap* heightMap = createImg(selectedBiome, hmSize);
				ImGui::End();
				ImGui::EndFrame();
				return heightMap;
			}

			ImGui::SliderInt("Map size", &hmSize, 25, 100);

			ImGui::Text("Selected biome");
			ImGui::SameLine();
			if (selectedBiome == 0) {
				if (ImGui::Button("Islands")) {
					selectedBiome = 1;
				}
			}
			else if (selectedBiome == 1) {
				if (ImGui::Button("Mountains")) {
					selectedBiome = 2;
				}
			}
			else if (selectedBiome == 2) {
				if (ImGui::Button("Ocean")) {
					selectedBiome = 0;
				}
			}

			

			ImGui::End();
		}
		else if (loadMap == 1) {
			ImGui::SetNextWindowSize(ImVec2(250, 250));
			ImGui::SetNextWindowPos(ImVec2((SCR_WIDTH / 2) - 250 / 2, (SCR_HEIGHT / 2) - 250 / 2));
			ImGui::Begin("Map loading", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |

				ImGuiWindowFlags_NoTitleBar);
			//ImGui::Begin("Map loading");
			//std::cout << totalMaps;
			for (int i = 0; i < totalMaps; i++) {
				std::string k = "map_";

				k.append(std::to_string(i));
				const char* ki = k.c_str();
				if (ImGui::Button(ki)) {
					selectedMap = i;
					loadMap = 0;
				}
			}
			if (ImGui::Button("Return")) {
				loadMap = 0;
			}
			ImGui::End();
		}
		else {
			ImGui::SetNextWindowSize(ImVec2(250, 250));
			ImGui::SetNextWindowPos(ImVec2((SCR_WIDTH / 2) - 250 / 2, (SCR_HEIGHT / 2) - 250 / 2));
			ImGui::Begin("How to use", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("To generate a terrain, select a \nsize and biome, then click\n'generate terrain'.\n\nTo load a PNG map, please name it \n'map_X' where x is a number\ncorresponding to (n-1) map files.\nThen select it and\npress 'load' button.\n\n\nPlease read readme file for\nmore detailed information. :)");
			if (ImGui::Button("Return")) {
				loadMap = 0;
			}
			ImGui::End();
		}
		// Rendering
		ImGui::Render();
		int display_w, display_h;

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//imgui
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
}

void startGame(GLFWwindow* window) {
	input = 1;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void initMapFiles() {
	//count all map files
	bool stopNow = false;
	//read maps from files
	do {
		//std::ifstream myReadFile;
		std::string mapName = "resources/map/map_";
		mapName.append(std::to_string(totalMaps));
		mapName.append(".png");
		
		//myReadFile.open(mapName);

		const char* nam = mapName.c_str();
		
		HeightMap* hm = new HeightMap(nam);
		if (hm->getHeight()) {
			totalMaps++;
		}
		else {
			stopNow = true;
		}


		/*if (myReadFile.is_open())
		{
			totalMaps++;
			myReadFile.close();
		}
		else {
			stopNow = true;
		}*/
	} while (!stopNow);
}

bool checkDead(Map* map, GLFWwindow* window, Player* player) {
	//loop through all spikes
	for (int i = 0; i < map->getSpikeCount(); i++) {
		//std::cout << map->convertToGridCoordinates(map->getSpikePos(i)).x << " " << map->convertToGridCoordinates(map->getSpikePos(i)).z << "\n";
		if (map->convertToGridCoordinates(player->getPos()).x == map->convertToGridCoordinates(map->getSpikePos(i)).x &&
			map->convertToGridCoordinates(player->getPos()).z == map->convertToGridCoordinates(map->getSpikePos(i)).z) {
			//kill player
			return true;
		}
	}
	return false;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!useCursor) {
		if (fov >= 15.0f && fov <= 45.0f)
			fov -= yoffset;
		if (fov <= 15.0f)
			fov = 15.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}
}