#include "environment.h"

void Environment::Init(GLuint width, GLuint height, ParticleGenerator *ps) {
	Width = width;
	Height = height;
	envModel = new Model("models/land.obj");
	particleSys = ps;  // need to initialize the particle system before initialize evironment !!!
}

void Environment::Update() {

}

void Environment::Draw(Shader &shader, Camera &camera) {

	int liveNum = particleSys->GetLiveNum();
	int nLights = min(100, liveNum); // maximum of lights: 100
	shader.Use();
	shader.SetVector3f("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetVector3f("viewPos", camera.Position);
	shader.SetFloat("lightProperty.constant", 3.0f);
	shader.SetFloat("lightProperty.linear", 0.9);
	shader.SetFloat("lightProperty.quadratic", 0.68);
	shader.SetVector3f("lightProperty.ambient", 0.000002f, 0.000002f, 0.000002f); // 调参项： 一只萤火虫的光能对环境光有多大影响？
	shader.SetVector3f("globalAmbient", 0.06f, 0.06f, 0.06f); // 调参项：全局环境光，营造气氛
	shader.SetVector3f("lightProperty.diffuse", 219.0 / 255, 1.0f, 47.0 / 255);
	shader.SetVector3f("lightProperty.specular", 130.0 / 255, 1.0f, 47.0 / 255);
	shader.SetInteger("activeLightNum", nLights);
	

	
	int i = 0, j = 0;
	while (j < nLights) {
		if (particleSys->particles[i].Life > 0) {
			char str[15];
			sprintf(str, "lightPos[%d]", j);
			shader.SetVector3f(str, particleSys->particles[i].Position);
			j++;
		}
		i++;
	}

	glm::mat4 view(1.0f);
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, -5.0f));
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	shader.SetMatrix4("model", model);

	envModel->Draw(shader);
}