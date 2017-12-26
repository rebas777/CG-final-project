#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "Camera.h"

// Represents a single particle and its state
struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;
	bool is_prime; // Prime ones are larger in size and self-luminous.
	int mode; // 0 -- firefly, 1 -- star, 2 -- snow
	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), is_prime(false), mode(0) {}
};

class ParticleGenerator {
public:

	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount, GLuint width, GLuint height);

	void Update(GLfloat dt, GLuint newParticles, glm::vec3 dstPos);

	void Draw(Camera &camera);

private:
	// State
	std::vector<Particle> particles;
	GLuint amount;
	// Render state
	Shader shader;
	Texture2D texture;
	GLuint VBO, VAO;
	GLuint Width, Height;
	glm::vec3 emitPos;

	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle &particle);
};
