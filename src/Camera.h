#pragma once

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "glm_include.h"

#include <cstdio>

#include "Gizmos.h"

class Camera
{

public:

	virtual bool Update();
	virtual void SetPerspective(float a_fieldOfView, float aspectRatio, float a_near, float a_far);
	virtual void SetLookAt(vec3 from, vec3 to, vec3 up);
	virtual void SetPosition(vec3 a_position);

	mat4 GetWorldTransform();
	mat4 GetView();
	mat4 GetProjection();
	mat4 GetProjectionView();

protected:

	mat4 worldTransform;
	mat4 viewTransform;
	mat4 projectionTransform;
	mat4 projectionViewTransform;

	void UpdateProjectionViewTransform();
private:
	
};

