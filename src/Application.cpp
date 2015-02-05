#include "Application.h"


Application::Application()
{
}


Application::~Application()
{
}

//bool Create(float screen_width, float screen_height, const char* window_title)
//{
//	printf("Creation Failed: Base Constructor Called");
//
//	return false;
//}

bool Application::StartUp()
{
	return true;
}


bool Application::ShutDown()
{
	return true;
}


bool Application::Update()
{
	return true;
}


bool Application::Draw()
{
	return true;
}