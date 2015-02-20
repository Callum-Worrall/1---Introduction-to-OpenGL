#define __ANIMATION_H__

#include "Application.h"
#include "FlyCamera.h"

#include "FBXFile.h"

#pragma once
class Animation : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void GenerateGLMeshes(FBXFile* fbx);

	void EvaluateSkeleton(FBXAnimation* animation, FBXSkeleton* skeleton, float timer, float fps);

	void UpdateBones(FBXSkeleton* skeleton);

private:
	FlyCamera*				m_camera;
	float					m_timer;

	//
	FBXFile*				m_file;

	//opengl handles that opengl needs to render stuff
	std::vector<OpenGLData> m_meshes;

	unsigned int m_program_ID;
};

