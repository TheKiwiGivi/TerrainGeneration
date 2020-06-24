
#include "stb_image.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ImageTexture.h"



ImageTexture::ImageTexture(std::string filePath, ImageType imageType)
{
	// Flip the image
	stbi_set_flip_vertically_on_load(true);

	// Loading the png file using stb_image.h
	if (imageType == png)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps

		//int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	else if (imageType == jpg)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
	    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data); 
	}

}

ImageTexture::~ImageTexture() 
{	
	glDeleteTextures(1, &id);
}


void ImageTexture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void ImageTexture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int ImageTexture::getID()
{
	return id;
}