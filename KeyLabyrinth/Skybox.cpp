#include "Skybox.h"



void Skybox::draw(glm::mat4 viewSky, glm::mat4 projectionSky)
{
	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);

	skyboxShader->setMat4("view", viewSky);
	skyboxShader->setMat4("projection", projectionSky);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}