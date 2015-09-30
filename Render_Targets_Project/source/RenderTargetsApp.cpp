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
	/*
	tut code here

	*/
	Gizmos::create();

	printf("Setting up FBO...\n");
	// setup and bind a framebuffer
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	// create a texture and bind it
	glGenTextures(1, &mFBOTexture);
	glBindTexture(GL_TEXTURE_2D, mFBOTexture);
	// specify texture format for storage
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// attach it to the framebuffer as the first color attachment
	// the FBO MUST still be bound
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mFBOTexture, 0);
	printf("Setting up render buffer...\n");
	// setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &mDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	// while the FBO is still bound
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

	// while the FBO is still bound
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	}
	else
	{
		printf("Framebuffer status complete...\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*
	setup plane that can use texture
	*/
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
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,	indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	
	/*
	setup shaders
	*/
	if (!shader.LoadShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH))
	{
		return false;
	}

	Gizmos::addSphere(vec3(0, 1, 0), .5f, 8, 8, vec4(1, 1, 0, 1));

	camera.StartupPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	camera.SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	return true;
}

void RenderTargetsApp::ShutDown()
{
	Gizmos::destroy();
	glfwDestroyWindow(window.handle);
	glfwTerminate();
}

bool RenderTargetsApp::Update()
{
	if (glfwWindowShouldClose(window.handle) || glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}

	glfwSwapBuffers(window.handle);
	glfwPollEvents();


	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	/*
	render to framebuffer
	*/

	// BACK BUFFER, FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glViewport(0, 0, 512, 512);

	glClearColor(0.75f, 0.75f, 0.75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw our meshes, or gizmos, to the render target
	Gizmos::draw(camera.GetViewProjection());


	// FRONT BUFFER? COLOR BUFFER?
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(camera.GetViewProjection()));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFBOTexture);

	int texUnit = 0;

	shader.SetUniform("diffuse", Shader::INT1, &texUnit);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	//Gizmos::draw(camera.GetViewProjection());	// draws 

	return true;
}

void RenderTargetsApp::Draw()
{
}
