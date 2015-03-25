#ifndef __POSTPROCESS_H__
#define __POSTPROCESS_H__

#include "Application.h"
#include "FlyCamera.h"


#pragma once
class PostProcess : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

	void Input();

	void GenerateFramebuffer();
	void GenerateScreenSpaceQuad();


private:

	unsigned int m_fbo;
	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;

	OpenGLData m_quad;
	unsigned int m_post_program;

	FlyCamera* m_camera;

	bool gridActive;
	bool gPressed;
};

#endif