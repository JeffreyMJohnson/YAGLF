#include "GLFramework.h"

#include <iostream>

typedef GLFramework glf;

void main()
{
	using namespace std;

	glf::Startup(100, 100, "foo", Color(1, 1, 1, 1));
	while (glf::Update())
	{
		cout << "tick\n";
	}
	
	glf::Cleanup();
}