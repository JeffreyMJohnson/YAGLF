//include your derived app class header here
//#include "MyGameApp.h"
#include "DeferredRenderingApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	DeferredRenderingApp* app = new DeferredRenderingApp();

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