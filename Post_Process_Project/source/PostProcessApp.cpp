#include "..\include\PostProcessApp.h"

typedef unsigned int uint;
typedef GLFramework glf;

bool PostProcessApp::StartUp()
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
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,	mFBOTexture, 0);
	glGenRenderbuffers(1, &mFBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mFBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mFBODepth);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// fullscreen quad
	glm::vec2 halfTexel = 1.0f / glm::vec2(1280, 720) * 0.5f;


	float vertexData[] = {
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y
	};
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	return true;
}

void PostProcessApp::ShutDown()
{

}

bool PostProcessApp::Update()
{
	Gizmos::clear();

	if (glfwWindowShouldClose(window.handle) || glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	//bind the target
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glViewport(0, 0, 1280, 720);
	//glClearColor(0.75f, 0.75f, .75f, 1);
	//clear the target
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw scene
	Gizmos::addSphere(vec3(0, 0, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));
	Gizmos::addRing(vec3(0, 0, 0), 1, 1.5, 100, vec4(1, 0, 0, 1));
	Gizmos::draw(camera.GetViewProjection());
	
	//bind back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//draw out full screen quad
	glUseProgram(shader.GetProgram());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFBOTexture);

	int loc = glGetUniformLocation(shader.GetProgram(), "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));

	glUniform1i(glGetUniformLocation(shader.GetProgram(), "target"), 0);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window.handle);
	glfwPollEvents();
	return true;
}

void PostProcessApp::Draw()
{
}
