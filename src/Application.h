#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#pragma once

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "glm_include.h"

#include <cstdio>

#include "Gizmos.h"

class Application
{
public:

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();

protected:

	GLFWwindow* m_window;

	int m_window_width;
	int m_window_height;

	float m_timer;


};

#endif