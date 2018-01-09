#include "game.h"
#include "resource_manager.h"


Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	sceneNum = 0;
	hasConstrain = true;
	finishing = false;
}

Game::~Game()
{

}

void Game::Init()
{
	// Load shaders
	// Shader for rendering the firefly itself
	ResourceManager::LoadShader("shaders/firefly.vs", "shaders/firefly.frag", nullptr, "lampShader");
	ResourceManager::LoadShader("shaders/bugShader.vs", "shaders/bugShader.frag", nullptr, "bugShader");

	// Shader for rendering the environment model
	ResourceManager::LoadShader("shaders/environment_color.vs", "shaders/environment_color_hdr.frag", nullptr, "environmentShader");
	ResourceManager::LoadShader("shaders/environment_texture.vs", "shaders/environment_texture.frag", nullptr, "environmentShader_real");

	// Shader for rendering the image from FBO to the whole screen
	ResourceManager::LoadShader("shaders/FBOscene.vs", "shaders/FBOscene.frag", nullptr, "sceneShader");

	// Shader for gaussian blur.
	ResourceManager::LoadShader("shaders/shaderBlur.vs", "shaders/shaderBlur.frag", nullptr, "shaderBlur");

	// Shader for blending origin image and the after-blur image.
	ResourceManager::LoadShader("shaders/shaderBloomFinal.vs", "shaders/shaderBloomFinal.frag", nullptr, "shaderBloomFinal");


	/*************************************************** Initialize the low poly firefly scene **************************************************/

	// Initialize particle generator
	Shader tmp1 = ResourceManager::GetShader("lampShader");
	Shader bugShader = ResourceManager::GetShader("bugShader");
	Texture2D nullTexture;
	particleSys = new ParticleGenerator(tmp1, bugShader, nullTexture, 500, Width, Height);
	glm::vec3 emitPos1, emitPos2, emitPos3;
	emitPos1 = glm::vec3(0.0f, 0.0f, 40.0f); // 三个方向各有一个发射点
	emitPos2 = glm::vec3(30.0f, 1.0f, 2.0f);
	emitPos3 = glm::vec3(-20.0f, -1.0f, 2.0f);
	particleSys->SetEmitPos(emitPos1, emitPos2, emitPos3);

	// Initialize  environment
	environment.Init(Width, Height, particleSys, "models/lowpolyland/part.obj");

	// 创建主帧缓冲，之后所有的粒子渲染都在这个帧缓冲里进行
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 为模糊处理创建两个基本的帧缓冲（只有颜色缓冲纹理）
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}


	sceneShader = ResourceManager::GetShader("sceneShader");
	shaderBlur = ResourceManager::GetShader("shaderBlur");
	shaderBloomFinal = ResourceManager::GetShader("shaderBloomFinal");

	sceneShader.Use();
	sceneShader.SetInteger("screenTexture", 0);
	shaderBlur.Use();
	shaderBlur.SetInteger("image", 0);
	shaderBloomFinal.Use();
	shaderBloomFinal.SetInteger("scene", 0);
	shaderBloomFinal.SetInteger("bloomBlur", 1);

	/************************************************* End initializing the low poly firefly scene ************************************************/


	/************************************************* Initializing the realistic scene ***********************************************************/
	environment_real.Init(Width, Height, particleSys, "models/island_with_texture/Small Tropical Island.obj");
	//environment_real.Init(Width, Height, particleSys, "models/ball.fbx");
	environment_real.terrainPos = glm::vec3(0.0f, -40.0f, -5.0f);
	//environment_real.terrainPos = glm::vec3(0.0f, 0.0f, 0.0f);
	environment_real.terrainScale = glm::vec3(0.4f, 0.4f, 0.4f);
	//environment_real.terrainScale = glm::vec3(4.0f, 4.0f, 4.0f);


	/************************************************ End initializing the realistic scene ********************************************************/

}

void Game::Update(GLfloat dt)
{

	if (this->finishing) {
		particleSys->DieUpdate(dt);
		return;
	}
	if (this->hasConstrain) {
		particleSys->Update(dt, 0.01, camera.Position);
	}
	else {
		particleSys->RandUpdate(dt, 0.01);
	}
	
		
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->Keys[GLFW_KEY_Q]) {
		this->sceneNum = 1;
		this->particleSys->isReal = true;
	}
	if (this->Keys[GLFW_KEY_E]) {
		this->hasConstrain = true;
	}
	if (this->Keys[GLFW_KEY_R]) {
		this->hasConstrain = false;
	}
	if (this->Keys[GLFW_KEY_T]) {
		if (sceneNum == 1) {
			this->finishing = true;
		}
	}
	if (this->Keys[GLFW_KEY_C]) {
		this->particleSys->ResetSystem();
	}
}

void Game::Render()
{

	if (sceneNum == 0) { // Render the low poly firefly scene
		// Step1: 把所有粒子渲染在帧缓冲里（以便后续的后处理）
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the environment
		Shader tmp2 = ResourceManager::GetShader("environmentShader");
		environment.Draw(tmp2, camera);

		// Draw the particle system
		particleSys->Draw(camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Step2: 从已经渲染好的帧缓冲里面取出图像进行 Two-pass Gaussian Blur
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 10; // 10 次高斯模糊
		shaderBlur.Use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			shaderBlur.SetInteger("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
			renderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//// Step1.5: 从帧缓冲中取出图像渲染到默认缓冲
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//sceneShader.Use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		//renderQuad();

		// Step3: 将帧缓冲中的原内容和高斯模糊过的内容渲染在一起，达到 bloom 的效果。
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderBloomFinal.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		shaderBloomFinal.SetInteger("bloom", true);
		shaderBloomFinal.SetFloat("exposure", 1.0f);
		renderQuad();
	}
	else { // Render the realistic scene

		   // Draw the environment
		Shader tmp6 = ResourceManager::GetShader("environmentShader_real");
		environment_real.Draw(tmp6, camera);

		// Draw the particle system
		particleSys->Draw(camera);
		

	}

}

void Game::renderQuad() {
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}