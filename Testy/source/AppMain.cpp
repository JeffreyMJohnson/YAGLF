//include your derived app class header here
//#include "MyGameApp.h"
#include "TestyApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	TestyApp* app = new TestyApp();

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