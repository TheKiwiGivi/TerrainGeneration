#pragma once
#include <string>

enum ImageType { png = 0, jpg = 1 };

class ImageTexture
{
	unsigned int id; // for rendering
	int width;
	int height;
	int nrChannels;


public:
	ImageTexture(std::string filePath, ImageType imageType);
	~ImageTexture();

public:
	void bind() const;
	void unbind();
	int getID();
}; 