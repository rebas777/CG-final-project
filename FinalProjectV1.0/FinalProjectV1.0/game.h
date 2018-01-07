#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "box.h"
#include "lightBox.h"
#include "particle_generator.h"
#include "model.h"
#include "environment.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	Camera camera;
	ParticleGenerator *particleSys;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);//运动逻辑
	void Render();//渲染游戏场景

	Environment environment;
	Environment environment_real;

private :

	GLuint sceneNum; // 0 -- low poly firefly scene;  1 -- realistic scene;
	bool hasConstrain;
	bool finishing; // particle system is dying, fading and generate secondaty particle system.

	// Framebuffer object for bloom process
	GLuint hdrFBO;
	GLuint colorBuffers[2];
	GLuint pingpongFBO[2];
	GLuint pingpongColorbuffers[2];
	GLuint quadVBO = 0, quadVAO = 0;

	Shader sceneShader;
	Shader shaderBlur;
	Shader shaderBloomFinal;


	void renderQuad();

	//Model *tmpModel;
};

#endif