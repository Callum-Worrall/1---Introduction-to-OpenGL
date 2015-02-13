#ifndef __TEXTURING_H__
#define __TEXTURING_H__

#include "Application.h"
#include "FlyCamera.h"

#pragma once

class Texturing : public Application
{
public:
	//Texturing();
	//~Texturing();

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void LoadTexture(const char* filename);
	void GenerateQuad(float size);

private:
	FlyCamera* m_camera;

	unsigned int m_texture;

	unsigned int m_program_ID;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

};

#endif