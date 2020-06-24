//works somehow
#define GLFW_INCLUDE_NONE
#include <iostream>
#include "input.h"
#include "map.h"
#include "key.h"
#include "enemy.h"
#include <glm/gtc/matrix_transform.hpp>

// IMGUI INCLUDES
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>

// function used to pause game
void pauseGame(GLFWwindow* window);

extern float deltaTime;
extern bool useCursor;

extern int cameraMode;
bool cursorSwitched = false;
//extern bool jumping;
//extern bool canJump;

bool cameraSwitched = false;

bool pausedReleasedGame = false;

//This enemy is for debug purposes
void processInput(GLFWwindow *window, Map* map, Player* player)
{
	if (cursorSwitched == false && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		useCursor = !useCursor;
		cursorSwitched = true;
	}
	if (cursorSwitched && glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
	{
		cursorSwitched = false;
	}
	if (cameraSwitched == false && glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		cameraSwitched = true;
		if (cameraMode == 0)
		{
			cameraMode = 1;
		}
		else
		{
			cameraMode = 0;
		}
	}
	if (cameraSwitched && glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
	{
		cameraSwitched = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{

		map->checkFoundKeys(player->getPos());

		//debug
		//std::cout << cameraPos.x << " " << cameraPos.z << std::endl;
	}

	// Pauses the game
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (pausedReleasedGame != false)
		{
			pausedReleasedGame = false;
			std::cout << "Pause game now" << std::endl;
			pauseGame(window);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
	{
		pausedReleasedGame = true;
	}

	glm::vec3 cameraPosTemp = player->getPos();

	float cameraSpeed = 0.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			cameraPosTemp += (cameraSpeed * player->getCamFront())*3.f;
		}
		else
		{
			cameraPosTemp += cameraSpeed * player->getCamFront();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosTemp -= cameraSpeed * player->getCamFront();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosTemp -= glm::normalize(glm::cross(player->getCamFront(), player->getCamUp())) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosTemp += glm::normalize(glm::cross(player->getCamFront(), player->getCamUp())) * cameraSpeed;


	//if (!jumping && canJump && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	//{
	//	jumping = true;
	//}


	int mapLengthX = map->getMapLengthX();
	int mapLengthZ = map->getMapLengthZ();

	//std::cout << player->getPos().x << " " << player->getPos().z << "\n";
	//if (map->notWall(cameraPosTemp.x, cameraPosTemp.z) && cameraPosTemp.x - 0.1 >= -float(mapLengthX) / 2 && cameraPosTemp.x + 0.1 <= float(mapLengthX) / 2)
	{
		player->setXPos(cameraPosTemp.x);
	}
	//if(!player->getWalking())
	
	
	//if (map->notWall(cameraPosTemp.x, cameraPosTemp.z) && cameraPosTemp.z - 0.1 >= -float(mapLengthZ) / 2 && cameraPosTemp.z + 0.1 <= float(mapLengthZ) / 2)
	{
		player->setZPos(cameraPosTemp.z);
	} 
	if (!player->getWalking()) {
		player->setYPos(cameraPosTemp.y);
	}
	else {
		float newYPosition = map->getYPos(cameraPosTemp.x, cameraPosTemp.z);
		//std::cout << newYPosition << "\n";
		player->setYPos(newYPosition+0.3f);
	}
}

int input2 = 0;

void pauseGame(GLFWwindow* window)
{
	bool paused = true;
	bool pausedReleased = false;


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	while (input2 != 1  && paused == true )
	{

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{

		if (pausedReleased == true)
		{
			paused = false;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
	{
		pausedReleased = true;
	}
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Framework! (paused)");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("Game is paused");
			if (ImGui::Button("Continue Game")) {
				paused = false;
			}
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;

				//imgui
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

	} //end while

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}