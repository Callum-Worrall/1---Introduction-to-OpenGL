#ifndef __SOLAR_SYSTEM_TEST_H__
#define __SOLAR_SYSTEM_TEST_H__


#include "Application.h"
#include "FlyCamera.h"

#pragma once

class SolarSystemTest : public Application
{
public:
	SolarSystemTest();
	~SolarSystemTest();

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	mat4 BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation);

private:

	FlyCamera* m_camera;

	float m_camera_x;
	float m_camera_z;

	mat4 m_view;
	mat4 m_projection;

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