#include "SolarSystemTest.h"
#include "CameraAndProjection.h"

int main()
{
	SolarSystemTest app;
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