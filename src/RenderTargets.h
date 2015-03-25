#ifndef RENDER_TARGET_H_
#define RENDER_TARGET_H_

#include "Application.h"
#include "FlyCamera.h"
#include "Vertex.h"


class RenderTargets : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();
	virtual bool Update();
	virtual bool Draw();

	void GenerateFramebuffer();
	void GeneratePlane();

	FlyCamera m_camera;

	unsigned int m_fbo;
	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;

	OpenGLData	m_plane;

	unsigned int m_program_id; //ADDED THIS

};

#endif //CAM_PROJ_H_