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

	// Initialize game objects
    box.Init(Width, Height);
	lightBox.Init(Width, Height);
}

void Game::Update(GLfloat dt)
{
	box.Update();
	lightBox.Update();
}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
	Shader tmp = ResourceManager::GetShader("lightingShader");
	box.Draw(tmp, camera);
	Shader tmp1 = ResourceManager::GetShader("lampShader");
	lightBox.Draw(tmp1, camera);
}