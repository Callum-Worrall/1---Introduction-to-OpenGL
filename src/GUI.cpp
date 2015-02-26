#include "GUI.h"
#include "Utility.h"


void OnMouseButton(GLFWwindow* window, int button, int pressed, int mod_keys)
{
	TwEventMouseButtonGLFW(button, pressed);
}

void OnMousePosition(GLFWwindow* window, double x, double y)
{
	TwEventMousePosGLFW((int)x, (int)y);
}

void OnMouseScroll(GLFWwindow* window, double x, double y)
{
	TwEventMouseWheelGLFW((int)y);
}

void OnKey(GLFWwindow* window, int key, int scancode, int pressed, int mod_keys)
{
	TwEventKeyGLFW(key, pressed);
}

void OnChar(GLFWwindow* window, unsigned int c)
{
	TwEventCharGLFW(c, GLFW_PRESS);
}

//When the Window gets resized
void OnWindowResize(GLFWwindow* window, int width, int height)
{
	TwWindowSize(width, height);
	glViewport(0, 0, width, height);
}

bool GUI::StartUp(const char* a_bar_name, int a_width, int a_height, GLFWwindow* a_window) // START UP //
{
	glfwSetMouseButtonCallback(a_window, OnMouseButton);
	glfwSetCursorPosCallback(a_window, OnMousePosition);
	glfwSetScrollCallback(a_window, OnMouseScroll);
	glfwSetCharCallback(a_window, OnChar);
	glfwSetWindowSizeCallback(a_window, OnWindowResize);

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(a_width, a_height);

	m_bar = TwNewBar(a_bar_name);

	return true;
}


//bool GUI::ShutDown()	// SHUT DOWN //
//{
//
//}

//bool GUI::Update()	// UPDATE //
//{
//
//}


bool GUI::Draw()	// DRAW //
{
	TwDraw();

	return true;
}

TwBar* GUI::GetBar()
{
	return m_bar;
}