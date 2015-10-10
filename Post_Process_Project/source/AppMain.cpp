//include your derived app class header here
//#include "MyGameApp.h"
#include "PostProcessApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	PostProcessApp* app = new PostProcessApp();

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