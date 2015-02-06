#include "Application.h"


Application::Application()
{
}


Application::~Application()
{
}


bool Application::StartUp()
{
	//Open Graphics Library Frame Work Initialized
	if (glfwInit() == false)
	{
		return false;
	}

	//create window x, y, title, monitor, share?
	m_window = glfwCreateWindow(m_window_width, m_window_height, "Empty Application", nullptr, nullptr);

	//Check that the window is alright to use
	if (m_window == nullptr)
	{
		return false;
	}

	//Make the Current Context the GLFW window
	glfwMakeContextCurrent(m_window);

	ogl_LoadFunctions();

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	//Print Version Name to Console
	int major_version = ogl_GetMajorVersion();
	int minor_version = ogl_GetMinorVersion();
	printf("Successfully loaded OpenGL version %d.%d\n", major_version, minor_version);


	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//glEnable(GL_DEPTH_TEST);

	return true;
}


bool Application::ShutDown()
{
	if (glfwWindowShouldClose(m_window) == true)
	{
		//Destroy the Window
		glfwDestroyWindow(m_window);
		
		//Termine the Open Graphics Library Framework
		glfwTerminate();

		return true;
	}

	return false;
}


bool Application::Update()
{
	if (glfwWindowShouldClose(m_window))
	{
		return false;
	}

	return true;
}


bool Application::Draw()
{
	return true;
}