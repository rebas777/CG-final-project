#include "game.h"
#include "resource_manager.h"


Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
}

Game::~Game()
{

}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("shaders/lamp.vs", "shaders/lamp.frag", nullptr, "lampShader");
	ResourceManager::LoadShader("shaders/lighting.vs", "shaders/lighting.frag", nullptr, "lightingShader");
	ResourceManager::LoadShader("shaders/environment_color.vs", "shaders/environment_color_hdr.frag", nullptr, "environmentShader");
	//ResourceManager::LoadShader("shaders/environment_texture.vs", "shaders/environment_texture.frag", nullptr, "environmentShader");

	// Initialize game objects
    box.Init(Width, Height);
	lightBox.Init(Width, Height);
	//tmpModel = new Model("models/car/Avent.obj");

	// Initialize particle generator
	Shader tmp1 = ResourceManager::GetShader("lampShader");
	Texture2D nullTexture;
	particleSys = new ParticleGenerator(tmp1, nullTexture, 500, Width, Height);
	glm::vec3 emitPos1, emitPos2, emitPos3;
	emitPos1 = glm::vec3(0.0f, 0.0f, 40.0f); // 三个方向各有一个发射点
	emitPos2 = glm::vec3(30.0f, 1.0f, 2.0f);
	emitPos3 = glm::vec3(-20.0f, -1.0f, 2.0f);
	particleSys->SetEmitPos(emitPos1, emitPos2, emitPos3);

	// Initialize  environment
	environment.Init(Width, Height, particleSys);
}

void Game::Update(GLfloat dt)
{
	//box.Update(camera.Position, particleSys->GetLiveNum());
	//lightBox.Update();
	particleSys->Update(dt, 0.002, camera.Position);
	//particleSys->Update(dt, 0.002, glm::vec3(0.0f, 0.0f, 15.0f));
}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
	//Shader tmp = ResourceManager::GetShader("lightingShader");
	//box.Draw(tmp, camera);
	//Shader tmp1 = ResourceManager::GetShader("lampShader");
	//lightBox.Draw(tmp1, camera);

	// Draw the environment
	Shader tmp2 = ResourceManager::GetShader("environmentShader");
	environment.Draw(tmp2, camera);

	// Draw the particle system
	particleSys->Draw(camera);
}