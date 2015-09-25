#include "..\include\RenderTargetsApp.h"

typedef unsigned int uint;

bool RenderTargetsApp::StartUp()
{
	glGenBuffers(1, &mFBO);
	glBindBuffer(GL_FRAMEBUFFER, mFBO);

	//attach render targets


	glBindBuffer(GL_FRAMEBUFFER, 0);
	return false;
}

void RenderTargetsApp::ShutDown()
{
}

bool RenderTargetsApp::Update()
{
	return false;
}

void RenderTargetsApp::Draw()
{
}
