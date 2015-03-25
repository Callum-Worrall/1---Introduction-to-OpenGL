#ifndef __SCENEMANAGEMENT_H__
#define __SCENEMANAGEMENT_H__

#include "Application.h"
#include "FlyCamera.h"

#include "BoundingSphere.h"
#include "AABB.h"


#pragma once
class SceneManagement : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();
	virtual bool Update();
	virtual bool Draw();

	void Input();

	void GetFrustumPlanes(const mat4 &transform, vec4* planes);


	MeshObject LoadMesh(char* obj_filename);
	void DrawMesh(MeshObject mesh);

private:
	bool gridActive;

	FlyCamera* m_camera;

	BoundingSphere sphere;

	// a plane that faces up
	// offset from (0,0,0) by 1
	vec4 plane;

	//Frustum Planes
	vec4 frustumPlanes[6];


	bool m_gPressed;


	//MeshObject(Struct) meshIndex[];



	bool inFrustum;
};

#endif


//MeshObject struct
//Altered AABB Version with AABB