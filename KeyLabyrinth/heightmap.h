#if !defined(__HEIGHTMAP_H)
#define __HEIGHTMAP_H
#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include "stb_image.h"

class HeightMap
{
private:
	int width;
	int height;
	unsigned char* data;
	unsigned long size;
	unsigned char min, max, range;

	float min2;
	float max2;
	float range2;

	bool notFailed = true;

	float bHighTreshold;
	float bTreshold;

	int nOutputWidth = 100;
	int nOutputHeight = 100;

	enum Biome {islands, mountains, ocean, loaded};

	Biome biome = islands;

	std::vector<std::vector<float>> heights;

public:
	HeightMap(int b, int scale) {
		nOutputWidth = scale;
		nOutputHeight = scale;
		if (b == 0) {
			biome = islands;
		}
		if (b == 1) {
			biome = mountains;
		}
		if (b == 2) {
			biome = ocean;
		}
		
		if (biome == islands) {
			bHighTreshold = 0.65f;
			bTreshold = 0.45f;
		} 
		else if (biome == ocean) {
			bHighTreshold = 1.0f;
			bTreshold = 1.0f;
		}
		else if (biome == mountains) {
			bHighTreshold = 0.45f;
			bTreshold = 0.25f;
		}
		
		int choice = 1;

			normalify2();
			scan2();
			display2();
		
	}

	HeightMap(const char* filePath) {
		//std::cout << "VIRKER IKKe\n";
		//biome = mountains;
		biome = loaded;
		int nrChannels;
		data = stbi_load(filePath, &width, &height, &nrChannels, 1);
		if (data)
		{
			size = width * height;
			scan();
			//display();
			loadFloats();
			showFloats();
		}
		else
		{
			if(!notFailed)std::cout << "Failed to load height map." << std::endl;
			notFailed = false;
		}
	}
	~HeightMap() {

	}

	int getNum(int x, int j) {
		return (x*width) +(j*width);
	}
	void normalify2() {
	
		//new
		// 2D noise variables
		
		float *fNoiseSeed2D = nullptr;
		float *fPerlinNoise2D = nullptr;


		int nOctaveCount = 5;
		float fScalingBias = 0.8f;
		int nMode = 1;


		fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
		fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];
		for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;



		perlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScalingBias, fPerlinNoise2D);
		std::vector<float> tmp;
		for (int x = 0; x < nOutputWidth; x++)
		{
			tmp.clear();
			for (int y = 0; y < nOutputHeight; y++)
			{
				float pixel_bw = (float)(fPerlinNoise2D[y * nOutputWidth + x]);
				//std::cout << pixel_bw << " ";
				if (pixel_bw <= bTreshold)pixel_bw = bTreshold;


				if (biome == mountains) {
					if (pixel_bw >= 0.55f && pixel_bw <= 0.60f) {
						pixel_bw = 0.86f+(float((rand() % 3) + 1)) / 100.0f;
					}
					if (pixel_bw > 0.60f && pixel_bw <= 0.70f) {
						pixel_bw = 0.90f + (float((rand() % 10) + 1)) / 100.0f;
					}
					if (pixel_bw > 0.45f && pixel_bw <= 0.60f) {
						
						pixel_bw = 0.65f+(float((rand()%10)+1))/100.0f;
					}
				}

				tmp.push_back(pixel_bw);

			}
			heights.push_back(tmp);
			//std::cout << "\n";
		}
		width = nOutputHeight;
		height = nOutputWidth;
		size = width * height;

		//old
		/*
		int nOctaveCount = 4;
		int nOutputWidth = 32;
		int nOutputHeight = 32;
		float fScalingBias = 2.0f;
		float *fNoiseSeed2D = nullptr;
		float *fPerlinNoise2D = nullptr;


		fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
		fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];

		for (int i = 0; i < nOutputWidth * nOutputHeight; i++) {
			fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
		}

		int nWidth = nOutputWidth;
		int nHeight = nOutputHeight;
		float* fSeed = fNoiseSeed2D;
		int nOctaves = nOctaveCount;
		float *fOutput = fPerlinNoise2D;

		//for 1D ONLY
		for (int x = 0; x < nWidth; x++){
			for (int y = 0; y < nHeight; y++) {
				float fNoise = 0.0f;
				float fScale = 1.0f;
				float fScaleAcc = 0.0f;
				float fBias = fScalingBias;


				for (int o = 0; o < nOctaves; o++) {
					int nPitch = nWidth >> o;
					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
					int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f, -fBlendX) * fSeed[nSampleY1 * nWidth * nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
					float fSampleB = (1.0f, -fBlendX) * fSeed[nSampleY2 * nWidth * nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScaleAcc += fScale;
					fScale = fScale / fBias;
				}
				fOutput[y * nWidth + x] = fNoise / fScaleAcc;
			}
	}

		std::vector<float> tmp;
		std::vector<std::vector<float>> tmp2;

		for (int i = 0; i < nOutputWidth; i++) {
		tmp.clear();
			for (int j = 0; j < nOutputHeight; j++) {
				tmp.push_back(fPerlinNoise2D[j * nOutputWidth + i]);
				std::cout << fPerlinNoise2D[j * nOutputWidth + i] << " ";
			}
			tmp2.push_back(tmp);
			std::cout << "\n";
		}
		heights = tmp2;
		size = nOutputHeight * nOutputWidth;
		width = nOutputWidth;
		height = nOutputHeight;
		//reversed

		*/

	}
	

	void perlinNoise2D(int nWidth, int nHeight, float *fSeed, int nOctaves, float fBias, float *fOutput)
	{
		// Used 1D Perlin Noise
		for (int x = 0; x < nWidth; x++)
			for (int y = 0; y < nHeight; y++)
			{
				float fNoise = 0.0f;
				float fScaleAcc = 0.0f;
				float fScale = 1.0f;

				for (int o = 0; o < nOctaves; o++)
				{
					int nPitch = nWidth >> o;
					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
					int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
					float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

					fScaleAcc += fScale;
					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScale = fScale / fBias;
				}

				// Scale to seed range
				fOutput[y * nWidth + x] = fNoise / fScaleAcc;
			}

	}



	void normalify() {
		int dividor = 1;
		int amountCheck = 6;
		int power = 8;

		std::vector<float> tmp;
		std::vector<std::vector<float>> tmp2;
		int counter = 0;
		for (int i = 0; i < height / dividor; i++) {
			tmp.clear();
			for (int j = 0; j < width / dividor; j++) {

				/*tmp.push_back((getData(i,j)+getData(i,j+1)+ getData(i, j-1) + getData(i+1, j) + getData(i - 1, j) + getData(i + 1, j + 1) + getData(i + 1, j - 1) + getData(i - 1, j - 1) + getData(i - 1, j + 1))/9);
				tmp.push_back((getData(i, j) + getData(i, j + 1) + getData(i, j - 1) + getData(i + 1, j) + getData(i - 1, j)+ getData(i+1, j+1) + getData(i+1, j - 1) + getData(i-1, j - 1) + getData(i - 1, j+1)) / 9);
				tmp.push_back((getData(i, j) + getData(i, j + 1) + getData(i, j - 1) + getData(i + 1, j) + getData(i - 1, j)+getData(i + 1, j + 1) + getData(i + 1, j - 1) + getData(i - 1, j - 1) + getData(i - 1, j + 1)) / 9);
				*/
				//tmp.push_back((getData(i, j) + getData(i, j + 1) + getData(i+1, j + 1) + getData(i + 1, j)) / 4);
				/*VIRKERtmp.push_back((getData(i, j) + getData(i, j + 1) + getData(i, j - 1) + getData(i + 1, j) + getData(i - 1, j) + getData(i + 1, j + 1) + getData(i + 1, j - 1) + getData(i - 1, j - 1) + getData(i - 1, j + 1)
				+ getData(i + 2, j) + getData(i + 2, j + 1) + getData(i +2, j + 2) + getData(i +2, j -1) + getData(i +2, j - 2) + getData(i + 1, j - 2) + getData(i , j-2)+ getData(i - 1, j - 2)
				+ getData(i - 2, j) + getData(i - 2, j + 1) + getData(i - 2, j +2 ) + getData(i - 2, j - 1) + getData(i - 2, j - 2) + getData(i - 1, j -2)+ getData(i, j + 2)+ getData(i, j +2)) / 25);*/
				//tmp.push_back((getData(i, j) + getData(i, j + 1) + getData(i, j - 1) + getData(i + 1, j) + getData(i - 1, j) + getData(i + 1, j + 1) + getData(i + 1, j - 1) + getData(i - 1, j - 1) + getData(i - 1, j + 1)) / 9);
				float total = 0.0f;
				for (int x = 0; x < amountCheck; x++) {
					for (int y = 0; y < amountCheck; y++) {
						total += getData((i + x) - amountCheck, (j + y) - amountCheck);
					}
				}
				total /= (2 * amountCheck + 1)*(2 * amountCheck + 1);
				total *= power;
				tmp.push_back(total);

			}
			tmp2.push_back(tmp);

			//std::cout << heights[0][0]<<"\n";
			//std::cout << tmp2[0][0] << "ned\n";
		}
		height /= dividor;
		width /= dividor;
		heights = tmp2;
	}

	void showFloats() {
		//std::cout << "dimensions: " << height << " " << width << "\n\n";
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				//std::cout << heights[i][j] << " ";
			}
			//std::cout << "\n";
		}
	}

	Biome getBiome() {
		return biome;
	}

	float* getNeighbourValues(int i, int j) {
		float k[4];
		k[0] = 1.0f;
		k[1] = 2.0f;
		k[2] = 3.0f;
		k[3] = 4.0f;
		return k;
	}
	float getNeighbourValues1(int i, int j) {
		float k = getData(i, j + 1);

		return k;
	}
	float getNeighbourValues2(int i, int j) {
		float k = getData(i, j);


		return k;
	}
	float getNeighbourValues3(int i, int j) {
		float k;
		k = 3.0f;

		return k;
	}
	float getNeighbourValues4(int i, int j) {
		float k;
		k = 4.0f;

		return k;
	}

	float getMin2() {
		return min2;
	}
	float getMax2() {
		return max2;
	}

	float getHighTreshold() {
		return bHighTreshold;
	}
	float getTreshold() {
		return bTreshold;
	}

	void loadFloats() {
		std::vector<float> tmp;
		// Loop over all rgb values inn the image
		int counter = 0;
		for (int i = 0; i < height; i++)
		{
			tmp.clear();
			for (int j = 0; j < width; j++)
			{
				tmp.push_back((float)data[counter++] / 255.0f); // normalize to 0, 1
																//std::cout << tmp[j] << " ";
			}
			//std::cout << "new line\n";
			heights.push_back(tmp);
			//std::cout << "\n";
		}
	}

	unsigned char* getData() {
		return data;
	}
	float getData(int i, int j) {
		if (i < height && i >= 0 && j < width && j >= 0) {
			return heights[i][j];
		}
		else {
			return 0.0f;
		}
	}

	int getSize() {
		return width * height;
	}

	void scan() {
		min = 255;
		max = 0;
		unsigned long k = 0;

		for (unsigned long i = 0; i < size; i++) {

			if (data[i] < min) {
				k = i;
				min = data[i];
			}
			if (data[i] > max) {
				max = data[i];
			}
		}
		range = max - min;


	}

	void scan2() {
		min2 = 1.0f;
		max2 = 0.0f;
		int k = 0;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				if (heights[i][i] < min2) {
					k = i;
					min2 = heights[i][j];
				}
				if (heights[i][j] > max2) {
					max2 = heights[i][j];
				}
			}
		}
		range2 = max2 - min2;
	}



	void display() {
		fprintf(stderr, "Width: %u\n", width);
		fprintf(stderr, "Height: %u\n", height);
		fprintf(stderr, "Size: %lu\n", size);
		fprintf(stderr, "Min: %d\n", min);
		fprintf(stderr, "Max: %d\n", max);
		fprintf(stderr, "Range: %d\n", range);

		for (unsigned long i = 0; i < size; i++) {
			//fprintf(stderr, "Range: %d\n", data[i]);
			//std::cout << data[i];
		}
	}
	void display2() {
		

		std::cout << "Width: " << width << "\n"
			<< "Height: " << height << "\n"
			<< "Min: " << min2 << "\n"
			<< "Max: " << max2 << "\n"
			<< "Range: " << range2 << "\n\n";


		for (unsigned long i = 0; i < size; i++) {
			//fprintf(stderr, "Range: %d\n", data[i]);
			//std::cout << data[i];
		}
	}

	int getHeight() {
		return height;
	}
	int getWidth() {
		return width;
	}
};

#endif