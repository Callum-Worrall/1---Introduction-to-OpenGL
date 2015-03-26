#ifndef __POSTPROCESS_H__
#define __POSTPROCESS_H__

#include "Application.h"
#include "FlyCamera.h"


#pragma once
class PostProcess : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void Input();

	void GenerateFramebuffer();
	void GenerateScreenSpaceQuad();

	//Planets
	mat4 BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation);

private:

	unsigned int m_fbo;
	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;

	OpenGLData m_quad;
	unsigned int m_post_program_id;

	FlyCamera* m_camera;

	bool gridActive;
	bool gPressed;

	float m_timer;

	//Planet Stuff
	vec4 white;
	vec4 black;
	vec4 red;
	vec4 blue;
	vec4 yellow;

	mat4 mat4_Sun;
	mat4 mat4_Planet1;
	mat4 mat4_Planet2;
	mat4 mat4_Planet3;
	mat4 mat4_Planet2Moon1;
};

#endif