#pragma once
#include "GameApp.h"
#include "Dryv3r.h"

class Test2App : public GameApp
{
public:
	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();

	GameObject testObject;
	int crateTexture = 0;
	GameObject test2;
};
