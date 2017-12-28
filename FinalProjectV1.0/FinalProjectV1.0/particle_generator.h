#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "Camera.h"

// Represents a single particle and its state
struct Particle {
	glm::vec3 Position, Velocity, Accelerate, randVelocity;
	glm::vec4 Color;
	GLfloat Life;
	GLfloat VelocityChangeCounter; // When the counter is done, change accelerate.
	bool is_prime; // Prime ones are larger in size and self-luminous.
	int mode; // 0 -- firefly, 1 -- star, 2 -- snow
	Particle() : Position(0.0f), Velocity(0.0f), Accelerate(0.0f), randVelocity(0.0f), Color(1.0f), Life(0.0f), is_prime(false), mode(0) {}
};

class ParticleGenerator {
public:

	std::vector<Particle> particles;

	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount, GLuint width, GLuint height);

	void Update(GLfloat dt, GLfloat newParticles, glm::vec3 dstPos);

	void Draw(Camera &camera);

	void SetEmitPos(glm::vec3 emitPos1, glm::vec3 emitPos2, glm::vec3 emitPos3);

	GLuint GetLiveNum();

	glm::vec3 getIthLivePos(int i);

private:
	// State
	
	GLuint amount;
	GLuint liveAmount;
	// Render state
	Shader shader;
	Texture2D texture;
	GLuint VBO, VAO;
	GLuint Width, Height;
	std::vector<glm::vec3> emitPoses;
	GLfloat newParticleCounter;

	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle &particle);
};
