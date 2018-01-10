#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "resource_manager.h"
#include "Camera.h"
#include "particle_generator.h"
#include "model.h"

class Environment {

public:
	Environment() {};
	void Init(GLuint width, GLuint height, ParticleGenerator *ps, GLchar *path);
	void  Draw(Shader &shader, Camera &camera, bool real);
	void Update();
	glm::vec3 terrainPos = glm::vec3(0.0f, -1.75f, -5.0f);
	glm::vec3 terrainScale = glm::vec3(1.2f, 1.2f, 1.2f);
	glm::vec3 lakePos = glm::vec3(0.0f, -5.0f, 0.0f);
	glm::vec3 lakeScale = glm::vec3(1000.0f, 1000.0f, 1000.0f);

private:
	Shader skyboxShader;
	Shader lakeShader;
	GLuint VBO, containerVAO, skyboxVAO, lakeVAO;
	GLuint Width, Height;
	ParticleGenerator *particleSys;
	// environment 从 particleSystem 读取光源数目和各个光源的位置
	
	Model *envModel;
	GLuint cubemapTexture;
	GLuint cubemapTexture_real;

};