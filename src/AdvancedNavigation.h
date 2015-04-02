#ifndef __AdvancedNavigation_H__
#define __AdvancedNavigation_H__

#include "Application.h"
#include "FlyCamera.h"

#include "tiny_obj_loader.h"

#pragma once

struct NavMeshNode
{
	//Polygon Details
	vec3 position;
	vec3 corners[3];
	NavMeshNode* edges[3];

	//A* Details
	unsigned int flags;
	float edge_costs;
};

class AdvancedNavigation : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void Input();

	void BuildNavigationMesh();

private:

	OpenGLData m_sponza;

	std::vector<NavMeshNode> m_nav_nodes;

	//FBXFile* m_sponza;

	GLuint m_program_ID;

	bool gridActive;

	FlyCamera* m_camera;

	bool gPressed;
};

#endif