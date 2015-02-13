#include "SolarSystemTest.h"
#include "CameraAndProjection.h"
#include "RenderingGeometry.h"
#include "Texturing.h"
#include "Lighting.h"
#include "OBJLoader.h"

int main()
{
	Texturing app;
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