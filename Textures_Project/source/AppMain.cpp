//include your derived app class header here
//#include "MyGameApp.h"
#include "TexturesApp.h"

void main()
{
	//instantiate derived app class on heap here
	//MyGameApp* app = new myGameApp();
	TexturesApp* app = new TexturesApp();

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