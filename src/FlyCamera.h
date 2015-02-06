#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera();
	FlyCamera(float width, float height, float something);
	FlyCamera(float aspect, float a_speed);

	~FlyCamera();

	bool Update(float deltaTime);

	void SetSpeed(float a_speed);

private:
	float m_speed;
	vec3 up;
};

