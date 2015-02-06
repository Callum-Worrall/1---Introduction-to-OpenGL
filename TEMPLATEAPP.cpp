#include "TEMPLATEAPP.h"


TEMPLATEAPP::TEMPLATEAPP()
{
}


TEMPLATEAPP::~TEMPLATEAPP()
{
}


bool TEMPLATEAPP::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	return true;
}


bool TEMPLATEAPP::ShutDown()
{
	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool TEMPLATEAPP::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	return true;
}

bool TEMPLATEAPP::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}