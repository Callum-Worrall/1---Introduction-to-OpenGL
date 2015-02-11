#include "SolarSystemTest.h"
#include "CameraAndProjection.h"
#include "RenderingGeometry.h"

int main()
{
	RenderingGeometry app;
	if (app.StartUp() == false)
	{
		return -1;
	}

	while (app.Update() == true)
	{
		app.Draw();
	}
	
	app.ShutDown();

	return 0;
}