#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera();
	~FlyCamera();

	FlyCamera(float width, float height, float a_speed, float a_sensitivity);

	bool Update(float deltaTime);

	void SetSpeed(float a_speed);
	void SetPerspective(float a_fieldOfView, float aspectRatio, float a_near, float a_far);
	void SetSensitivity(float a_sensitivity);

private:
	float m_speed;
	float m_sensitivity;
	vec3 up;
};

