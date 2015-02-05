#include "SolarSystemTest.h"

int main()
{
	SolarSystemTest* SolarSystemGame = new SolarSystemTest();
	if (SolarSystemGame->Create(1280.0f, 720.0f, "Computer Graphics - Ex Nihilo") == true)
	{
		while (SolarSystemGame->Update() == true)
		{
			SolarSystemGame->Draw();
		}
		SolarSystemGame->ShutDown();
	}
	delete SolarSystemGame;
	return 0;
}