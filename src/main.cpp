#include "SolarSystemTest.h"	//working
#include "CameraAndProjection.h"	//working
#include "RenderingGeometry.h"	//working
#include "Texturing.h"	//not working
#include "AdvancedTexturing.h"	//working
#include "OBJLoader.h"	//working - formerly known as lighting
#include "Quaternion.h"	//working
#include "Animation.h"	//not working - missing fbxloader assumed
#include "Particles.h"	//working, also contains GUI
#include "GPUParticles.h"	//working, also contains GUI
#include "SceneManagement.h"	//working

#include "RenderTargets.h"	//working
#include "PostProcess.h"	//not working (greyed out)
#include "DeferredRendering.h" //to do

int main()
{
	PostProcess app;
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