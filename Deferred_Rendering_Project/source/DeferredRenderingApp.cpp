#include "..\include\DeferredRenderingApp.h"

typedef unsigned int uint;
typedef GLFramework glf;

bool DeferredRenderingApp::StartUp()
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

	//load shaders
	if (!mGBufferShader.LoadShader((SHADER_PATH + std::string("gbuffer_vert.glsl")).c_str(), (SHADER_PATH + std::string("gbuffer_frag.glsl")).c_str()))
	{
		return false;
	}
	if (!mCompositeShader.LoadShader((SHADER_PATH + std::string("composite_vert.glsl")).c_str(), (SHADER_PATH + std::string("composite_frag.glsl")).c_str()))
	{
		return false;
	}
	if (!mDirectionalLightShader.LoadShader((SHADER_PATH + std::string("directional_light_vert.glsl")).c_str(), (SHADER_PATH + std::string("directional_light_frag.glsl")).c_str()))
	{
		return false;
	}

	camera.StartupPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	camera.SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);

	// setup gpass framebuffer
	glGenFramebuffers(1, &mGPassFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mGPassFBO);
	glGenTextures(1, &mAlbedoTexture);
	glBindTexture(GL_TEXTURE_2D, mAlbedoTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenTextures(1, &mPositionTexture);
	glBindTexture(GL_TEXTURE_2D, mPositionTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenTextures(1, &mNormalTexture);
	glBindTexture(GL_TEXTURE_2D, mNormalTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenRenderbuffers(1, &mGPassDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mGPassDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mAlbedoTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, mPositionTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, mNormalTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mGPassDepth);
	GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, gpassTargets);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error setting up GPass frame buffer.\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// setup light framebuffer
	glGenFramebuffers(1, &mLightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mLightFBO);
	glGenTextures(1, &mLightTexture);
	glBindTexture(GL_TEXTURE_2D, mLightTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mLightTexture, 0);
	GLenum lightTargets[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightTargets);
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error setting up light frame buffer.\n");
	}
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

	glGenVertexArrays(1, &mQuad.vao);
	glBindVertexArray(mQuad.vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//load model;
	//FBXFile file;
	std::string path = "../resources/models/fbx/soulspear.fbx";
	LoadModel(path);

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load("../resources/textures/soulspear/soulspear_diffuse.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	switch (imageFormat)
	{
	case 1: imageFormat = GL_RED; break;
	case 2: imageFormat = GL_RG; break;
	case 3: imageFormat = GL_RGB; break;
	case 4: imageFormat = GL_RGBA; break;
	}

	if (data == nullptr)
	{
		std::cout << "error loading texture.\n" << stbi_failure_reason();
	}
	
	glGenTextures(1, &spear_diffuse);
	glBindTexture(GL_TEXTURE_2D, spear_diffuse);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, imageWidth, imageHeight, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	//implement ambient lighting
	ambientLight.ambient = vec3(1,1,0);



	return true;
}

void DeferredRenderingApp::ShutDown()
{
	mFBX.unload();
}

bool DeferredRenderingApp::Update()
{
	Gizmos::clear();

	if (glfwWindowShouldClose(window.handle) || glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	// G-Pass: render out the albedo, position and normal
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, mGPassFBO);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(mGBufferShader.GetProgram());
	// bind camera transforms
	int loc = glGetUniformLocation(mGBufferShader.GetProgram(), "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));
	loc = glGetUniformLocation(mGBufferShader.GetProgram(), "View");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
	loc = glGetUniformLocation(mGBufferShader.GetProgram(), "albedoTex");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spear_diffuse);
	loc = glGetUniformLocation(mGBufferShader.GetProgram(), "ambientLight");
	glUniform3fv(loc, 1, glm::value_ptr(ambientLight.ambient));

	// draw our scene, in this example just the Stanford Bunny

	for (unsigned int i = 0; i < mSceneGeometry.size(); ++i)
	{
		
		glBindVertexArray(mSceneGeometry[i].vao);
		glDrawElements(GL_TRIANGLES, mSceneGeometry[i].triCount, GL_UNSIGNED_INT, 0);
	}

	// Light Pass: render lights as geometry, sampling position and
	// normals disable depth testing and enable additive blending
	glBindFramebuffer(GL_FRAMEBUFFER, mLightFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glUseProgram(mDirectionalLightShader.GetProgram());
	loc = glGetUniformLocation(mDirectionalLightShader.GetProgram(), "positionTexture");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mPositionTexture);
	loc = glGetUniformLocation(mDirectionalLightShader.GetProgram(), "normalTexture");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mNormalTexture);
	// draw lights as fullscreen quads
	drawDirectionalLight(glm::vec3(-1), glm::vec3(1));
	glDisable(GL_BLEND);

	// Composite Pass: render a quad and combine albedo and light
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(mCompositeShader.GetProgram());
	loc = glGetUniformLocation(mCompositeShader.GetProgram(), "albedoTexture");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mAlbedoTexture);
	loc = glGetUniformLocation(mCompositeShader.GetProgram(), "lightTexture");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mLightTexture);
	glBindVertexArray(mQuad.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window.handle);
	glfwPollEvents();
	return true;
}

void DeferredRenderingApp::Draw()
{
}

void DeferredRenderingApp::LoadModel(std::string path)
{
	FBXFile file;

	bool success = file.load(path.c_str(), FBXFile::UNITS_METER, false, false, false);
	if (!success)
	{
		std::cout << "Error loading FBX file: " << path << std::endl;
	}

	for (int i = 0; i < file.getMeshCount(); i++ )
	{
		FBXMeshNode* mesh = file.getMeshByIndex(i);
		GameObject rendObj;
		glGenVertexArrays(1, &rendObj.vao);
		glBindVertexArray(rendObj.vao);

		rendObj.triCount = mesh->m_indices.size();
		GLuint vbo, ibo;
		std::vector<Vertex> vertices;
		for each (auto vert in mesh->m_vertices)
		{
			Vertex v;
			v.color = vert.colour;
			v.normal = vert.normal;
			v.position = vert.position;
			v.tangent = vert.tangent;
			v.UV = vert.texCoord1;
			vertices.push_back(v);
		}
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, rendObj.triCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, rendObj.triCount * sizeof(uint), mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);//position
		glEnableVertexAttribArray(1);//color in shader right now.
		glEnableVertexAttribArray(2);//normal
		glEnableVertexAttribArray(3);//tangent
		glEnableVertexAttribArray(4);//UV coord

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 4));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		mSceneGeometry.push_back(rendObj);
		
	}
	file.unload();
}

void DeferredRenderingApp::drawDirectionalLight(const glm::vec3& direction, const glm::vec3& diffuse)
{
	glm::vec4 viewSpaceLight = camera.GetView() * glm::vec4(glm::normalize(direction), 0);
	int loc = glGetUniformLocation(mDirectionalLightShader.GetProgram(), "lightDirection");
	glUniform3fv(loc, 1, &viewSpaceLight[0]);
	loc = glGetUniformLocation(mDirectionalLightShader.GetProgram(), "lightDiffuse");
	glUniform3fv(loc, 1, &diffuse[0]);
	glBindVertexArray(mQuad.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

