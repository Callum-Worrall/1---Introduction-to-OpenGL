#ifndef __GUI_H__
#define __GUI_H__

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "AntTweakBar.h"

#pragma once
class GUI
{
public:

	bool StartUp(const char* a_bar_name, int a_width, int a_height, GLFWwindow* a_window);

	bool Draw();

	TwBar* GetBar();

private:

	TwBar* m_bar;
};

#endif
