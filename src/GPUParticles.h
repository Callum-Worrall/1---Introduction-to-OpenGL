#include "Application.h"
#include "FlyCamera.h"

#include "GUI.h"
#include "GPUEmitter.h"


#pragma once
class GPUParticles : public Application
{
public:
	GPUParticles();
	~GPUParticles();

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();
	float m_Timer;

private:
	FlyCamera* m_camera;


	GPUPointEmitter* m_emitter;

	GUI* m_emitterControl;

	vec4 m_background_color;
};

