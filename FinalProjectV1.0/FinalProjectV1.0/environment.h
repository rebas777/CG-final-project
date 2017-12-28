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
	void Init(GLuint width, GLuint height, ParticleGenerator *ps);
	void  Draw(Shader &shader, Camera &camera);
	void Update();

private:
	Shader skyboxShader;
	GLuint VBO, containerVAO, skyboxVAO;
	GLuint Width, Height;
	ParticleGenerator *particleSys;
	// environment 从 particleSystem 读取光源数目和各个光源的位置
	
	Model *envModel;
	GLuint cubemapTexture;

};