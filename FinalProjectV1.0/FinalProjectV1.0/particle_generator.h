#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <irrKlang/irrKlang.h>

#include "shader.h"
#include "texture.h"
#include "Camera.h"
#include "model.h"

// Represents a single particle and its state
struct Particle {
	glm::vec3 Position, Velocity, Accelerate, randVelocity;
	glm::vec3 Color;
	GLfloat Life;
	GLfloat distance;
	GLfloat VelocityChangeCounter; // When the counter is done, change accelerate.
	bool is_prime; // Prime ones are larger in size and self-luminous.
	int mode; // 0 -- firefly, 1 -- star, 2 -- snow
	int parent; // 次级粒子系统的标志，如果为一级粒子则为-1，如果为次级粒子则为其父亲的序号。
	Particle() : Position(0.0f), Velocity(0.0f), Accelerate(0.0f), randVelocity(0.0f),
		parent(-1), Color(1.0f), Life(0.0f), distance(0.0), is_prime(false), mode(0) {}

	
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISound *music;
  
};

class ParticleGenerator {
public:

	std::vector<Particle> particles;

	std::vector<Particle> sonParticles;

	ParticleGenerator(Shader shader, Shader bugShader, Texture2D texture, GLuint amount, GLuint width, GLuint height);

	void Update(GLfloat dt, GLfloat newParticles, glm::vec3 dstPos);

	void RandUpdate(GLfloat dt, GLfloat newParticles);

	void DieUpdate(GLfloat dt);	

	bool dying;

	bool isReal;

	void Draw(Camera &camera);

	void SetEmitPos(glm::vec3 emitPos1, glm::vec3 emitPos2, glm::vec3 emitPos3);

	GLuint GetLiveNum();

	glm::vec3 getIthLivePos(int i);

	void ResetSystem(); 

private:
	// State
	
	GLuint amount;
	GLuint amount_son;
	GLuint liveAmount;
	// Render state
	Shader shader;
	Shader bugShader;
	Texture2D texture;
	GLuint VBO, VAO;
	GLuint Width, Height;
	std::vector<glm::vec3> emitPoses;
	GLfloat newParticleCounter;
	Model *bugModel;
	
	

	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	GLuint firstUnusedParticle_son();
	// Respawns particle
	void respawnParticle(Particle &particle);
	void respawnParticle_son(Particle &particle, int parentNum);
};
