#ifndef __ADVANCED_TEXTURING_H__
#define __ADVANCED_TEXTURING_H__

#include "Application.h"
#include "FlyCamera.h"

#pragma once

class AdvancedTexturing : public Application
{
public:

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void LoadTextures(const char* a_diffuse_file,
						const char* a_normal_file,
						const char* a_specular_file);
	void GenerateQuad(float size);



private:
	FlyCamera* m_camera;
	unsigned int m_program_ID;

	OpenGLData m_quad;

	unsigned int m_diffuse_texture;
	unsigned int m_normal_texture;
	unsigned int m_specular_texture;


	vec3 m_ambient_light;
	vec3 m_light_dir;
	vec3 m_light_color;
	vec3 m_material_color;
	float m_specular_power;


	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
};

#endif