#pragma once
#include "Dryv3r.h"

class GameApp
{
public:

	virtual bool StartUp() = 0;
	virtual void ShutDown() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;
};
