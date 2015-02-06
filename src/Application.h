//#ifndef Application
//#define Application

#pragma once

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include <cstdio>

#include "Gizmos.h"

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec1;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Application
{
public:
	Application();
	virtual ~Application();

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