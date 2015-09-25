//include your derived app class header here
//#include "MyGameApp.h"
#include "RenderTargetsApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	RenderTargetsApp* app = new RenderTargetsApp();

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