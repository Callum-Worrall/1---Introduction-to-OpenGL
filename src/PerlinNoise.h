#ifndef __PERLIN_NOISE_H__
#define __PERLIN_NOISE_H__

#include "Application.h"
#include "FlyCamera.h"
#include "GUI.h"


#pragma once
class PerlinNoise : public Application
{
public:

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void Input();

	//Perlin Noise and Plane Building Functions
	void BuildGrid(vec2 real_dims, glm::ivec2 dims);
	void BuildPerlinTexture(glm::ivec2 dims, int octaves, float persistence);

private:

	FlyCamera* m_camera;

	bool gridActive;
	bool gPressed;

	//Perlin Noise and Plane Building Variables
	OpenGLData m_plane_mesh;
	GLuint m_program_id;
	GLuint m_perlin_texture;
	float* m_perlin_data;


	float m_dimX;
	float m_dimY;

	glm::ivec2 m_dims;
	int m_octaves;
	float m_persistence;
	float m_scale;

	glm::ivec2 lastDims;
	int lastOctaves;
	float lastPersistence;

	GUI TerrainWindow;


	float MIN;
	float MAX;
};

#endif