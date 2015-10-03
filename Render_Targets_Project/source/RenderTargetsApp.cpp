#include "..\include\RenderTargetsApp.h"

typedef unsigned int uint;
typedef GLFramework glf;

bool RenderTargetsApp::StartUp()
{
	//startup
	if (!glfwInit())
	{
		return false;
	}

	window.width = WINDOW_WIDTH;
	window.height = WINDOW_HEIGHT;
	window.title = WINDOW_TITLE;

	window.handle = glfwCreateWindow(window.width, window.height, window.title, nullptr, nullptr);

	if (nullptr == window.handle)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window.handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window.handle);
		glfwTerminate();
		return false;
	}

	window.clearColor = CLEAR_COLOR;
	glClearColor(window.clearColor.r, window.clearColor.g, window.clearColor.b, window.clearColor.a);

	Gizmos::create();

	if (!shader.LoadShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH))
	{
		return false;
	}

	camera.StartupPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	camera.SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);

	// setup framebuffer
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &mFBOTexture);
	glBindTexture(GL_TEXTURE_2D, mFBOTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		mFBOTexture, 0);
	glGenRenderbuffers(1, &mFBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mFBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, mFBODepth);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float vertexData[] = {
		-5, 0, -5, 1, 0, 0,
		5, 0, -5, 1, 1, 0,
		5, 10, -5, 1, 1, 1,
		-5, 10, -5, 1, 0, 1,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,	indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	

	



	return true;
}

void RenderTargetsApp::ShutDown()
{

}

bool RenderTargetsApp::Update()
{
	Gizmos::clear();

	if (glfwWindowShouldClose(window.handle) || glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glViewport(0, 0, 512, 512);
	glClearColor(0.75f, 0.75f, .75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::addSphere(vec3(0, 0, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));
	// draw our meshes, or gizmos, to the render target
	Gizmos::draw(camera.GetViewProjection());
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	glClearColor(0.25f, 0.25f, .25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));
	//Gizmos::draw(camera.GetViewProjection());
	//shader.SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(camera.GetViewProjection()));
	int loc = glGetUniformLocation(shader.GetProgram(), "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFBOTexture);
	//uint texSlot = 0;
	//shader.SetUniform("diffuse", Shader::INT1, &texSlot);
	glUniform1i(glGetUniformLocation(shader.GetProgram(), "diffuse"), 0);

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glfwSwapBuffers(window.handle);
	glfwPollEvents();
	return true;
}

void RenderTargetsApp::Draw()
{
}
