// IMGUI INCLUDES
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
extern int input;

void gameMenu(GLFWwindow* window)
{
	while (input != 1)
	{
		// update input
		processInput(window);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Key Labyrinth!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("Currently selected map: map_%d", selectedMap);               // Display some text (you can use a format strings too)
			if (ImGui::Button("How to play")) {
				//display how to play
			}
			if (ImGui::Button("Start")) {
				startGame(window);
			}
			if (ImGui::Button("Select Map")) {
				//load select map options
			}

			ImGui::SameLine();
			//ImGui::Text("counter = %d", counter);

			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// draw

		// end draw

		// render here
		//imgui
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

	} //end while

}

void startGame(GLFWwindow* window) {
	input = 1;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}