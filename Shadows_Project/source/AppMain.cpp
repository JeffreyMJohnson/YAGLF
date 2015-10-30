//include your derived app class header here
//#include "MyGameApp.h"
#include "ShadowsApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	ShadowsApp* app = new ShadowsApp();
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