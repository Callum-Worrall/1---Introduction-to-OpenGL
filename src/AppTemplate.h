#ifndef __APPTEMPLATE_H__
#define __APPTEMPLATE_H__

#include "Application.h"
#include "FlyCamera.h"


#pragma once
class AppTemplate : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void Input();


private:
	bool gridActive;

	FlyCamera* m_camera;
	
	bool gPressed;
};

#endif