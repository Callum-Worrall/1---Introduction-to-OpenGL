#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Application.h"
#include "FlyCamera.h"
#include "Emitter.h"

#include "GUI.h"

class Particles : public Application
{
public:
	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

private:
	FlyCamera* m_camera;

	Emitter* m_emitter;

	//GUI
	TwBar* m_bar;

	bool m_draw_gizmos;

	float m_fps;

	

	//
	unsigned int m_program_ID;

	GUI* m_emitterControl;

	vec4 m_background_color;


};



//crossproduct(billboards forward vector & cameras up vector) = perpendicular/right vector of billboard

//crossproduct(the right and forward axis of the billboards vector) = billboards up vector

#endif