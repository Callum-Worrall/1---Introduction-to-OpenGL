#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Application.h"
#include "FlyCamera.h"

struct Keyframe
{
	vec3 position;
	glm::quat rotation;
	vec3 scale;
};


#pragma once
class Quaternion : public Application
{
public:

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

private:
	FlyCamera* m_camera;

	Keyframe m_hip_frames[2];
	Keyframe m_knee_frames[2];
	Keyframe m_ankle_frames[2];

	mat4 hip_bone;
	mat4 knee_bone;
	mat4 ankle_bone;

	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

	float m_timer;
};

#endif