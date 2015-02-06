#include "Application.h"
#include "FlyCamera.h"

#pragma once
class CameraAndProjection : public Application
{
public:
	CameraAndProjection();
	~CameraAndProjection();

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

private:
	FlyCamera* m_camera;
};

