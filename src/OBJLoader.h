#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__

#include "Application.h"
#include "FlyCamera.h"

#include <vector>

#include "tiny_obj_loader.h"

#pragma once
class OBJLoader : public Application
{
public:

	OBJLoader();
	~OBJLoader();

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
	void CleanUpOpenGLBuffers();

	void ReloadShader();

private:
	FlyCamera m_camera;

	std::vector<OpenGLData> m_gl_data;

	unsigned int m_program_ID;

	vec3 m_ambient_light;
	vec3 m_light_dir;
	vec3 m_light_color;
	vec3 m_material_color;
	float m_specular_power;
};

#endif
