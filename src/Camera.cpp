#include "Camera.h"

bool Camera::Update()
{
	return true;
}

void Camera::SetPerspective(float a_fieldOfView, float a_aspectRatio, float a_near, float a_far)
{
	projectionTransform = glm::perspective(a_fieldOfView, a_aspectRatio, a_near, a_far);
	UpdateProjectionViewTransform();
}

void Camera::SetLookAt(vec3 a_from, vec3 a_to, vec3 a_up)
{
	viewTransform = glm::lookAt(a_from, a_to, a_up);
	worldTransform = glm::inverse(viewTransform);
	UpdateProjectionViewTransform();
}

void Camera::SetPosition(vec3 a_position)
{
	worldTransform[3][0] = a_position[0];
	worldTransform[3][1] = a_position[1];
	worldTransform[3][2] = a_position[2];
}


mat4 Camera::GetWorldTransform()
{
	return worldTransform;
}

mat4 Camera::GetView()
{
	return viewTransform;
}

mat4 Camera::GetProjection()
{
	return projectionTransform;
}

mat4 Camera::GetProjectionView()
{
	return projectionViewTransform;
}

void Camera::UpdateProjectionViewTransform()
{
	projectionViewTransform = projectionTransform * viewTransform;
}