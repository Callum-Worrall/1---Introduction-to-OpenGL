#include "SolarSystemTest.h"
#include "CameraAndProjection.h"
#include "RenderingGeometry.h"
#include "Texturing.h"
#include "AdvancedTexturing.h"
#include "Lighting.h"
#include "OBJLoader.h"
#include "Quaternion.h"
#include "Animation.h"
#include "Particles.h"
#include "GPUParticles.h"
#include "SceneManagement.h"

int main()
{
	SceneManagement app;
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