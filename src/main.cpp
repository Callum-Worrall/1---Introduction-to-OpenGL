#include "SolarSystemTest.h"
#include "CameraAndProjection.h"
#include "RenderingGeometry.h"
#include "Texturing.h"
#include "AdvancedTexturing.h"
#include "Lighting.h"
#include "OBJLoader.h"

int main()
{
	AdvancedTexturing app;
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