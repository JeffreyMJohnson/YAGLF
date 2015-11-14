//include your derived app class header here
//#include "MyGameApp.h"
#include "ParticlesApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	ParticlesApp* app = new ParticlesApp();

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