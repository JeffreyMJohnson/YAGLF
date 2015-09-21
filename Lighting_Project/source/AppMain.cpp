//include your derived app class header here
//#include "MyGameApp.h"
#include "LightingApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	LightingApp* app = new LightingApp();
	if (app->StartUp())
	{
		while (app->Update())
		{
			app->Draw();
		}
		app->ShutDown();
	}
	delete app;
	return;
}