#ifndef __RENDERING_GEO_H__
#define __RENDERING_GEO_H__


#include "Application.h"
#include "FlyCamera.h"

#pragma once
class RenderingGeometry : public Application
{
public:
	//RenderingGeometry();
	//~RenderingGeometry();

	virtual bool StartUp();
	virtual bool ShutDown();
	virtual bool Update();
	virtual bool Draw();

	void GenerateGrid(unsigned int rows, unsigned int cols);
	void GenerateShader();

private:
	FlyCamera* m_camera;

	unsigned int m_program_ID;
	unsigned int m_index_count;

	unsigned int m_VAO; // Vertex Array Object
	unsigned int m_VBO; // Vertex Buffer Object
	unsigned int m_IBO; // Index Buffer Object

};


#endif