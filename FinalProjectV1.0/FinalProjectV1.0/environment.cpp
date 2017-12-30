#include "environment.h"

GLuint loadCubemap(vector<const GLchar*> faces);

void Environment::Init(GLuint width, GLuint height, ParticleGenerator *ps) {
	Width = width;
	Height = height;
	//envModel = new Model("models/land.obj");
	envModel = new Model("models/lowpolyland/part.obj");
	particleSys = ps;  // need to initialize the particle system before initialize evironment !!!

	// Initialize skybox
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// Setup skybox VAO
	GLuint skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("skyboxes/skybox1/rt.jpg");
	faces.push_back("skyboxes/skybox1/lf.jpg");
	faces.push_back("skyboxes/skybox1/up.jpg");
	faces.push_back("skyboxes/skybox1/dn.jpg");
	faces.push_back("skyboxes/skybox1/bk.jpg");
	faces.push_back("skyboxes/skybox1/ft.jpg");
	cubemapTexture = loadCubemap(faces);

	ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.frag", nullptr, "skyboxShader");
	skyboxShader = ResourceManager::GetShader("skyboxShader");
}

void Environment::Update() {

}

void Environment::Draw(Shader &shader, Camera &camera) {

	// Draw skybox
	glDepthMask(GL_FALSE);
	skyboxShader.Use();
	glm::mat4 view_skybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection_skybox = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	skyboxShader.SetMatrix4("view", view_skybox);
	skyboxShader.SetMatrix4("projection", projection_skybox);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	skyboxShader.SetInteger("skybox", 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	//int liveNum = particleSys->GetLiveNum();
	//int nLights = min(100, liveNum); // maximum of lights: 100
	shader.Use();
	shader.SetVector3f("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetVector3f("viewPos", camera.Position);
	shader.SetFloat("lightProperty.constant", 3.0f);
	shader.SetFloat("lightProperty.linear", 0.9);
	shader.SetFloat("lightProperty.quadratic", 0.68);
	shader.SetVector3f("lightProperty.ambient", 0.000002f, 0.000002f, 0.000002f); // 调参项： 一只萤火虫的光能对环境光有多大影响？
	//shader.SetVector3f("globalAmbient", 0.06f, 0.06f, 0.06f); // 调参项：全局环境光，营造气氛
	shader.SetVector3f("globalAmbient", 0.14f, 0.14f, 0.14f); // 调参项：全局环境光，营造气氛
	shader.SetVector3f("lightProperty.diffuse", 219.0 / 255, 1.0f, 47.0 / 255);
	shader.SetVector3f("lightProperty.specular", 130.0 / 255, 1.0f, 47.0 / 255);
	

	int j = 0, nCenter = 0; // nCenter means how many individials is close enough to the attraction
	for (Particle particle : particleSys->particles) {
		if (particle.Life > 0) {
			if (particle.distance > 4.0) { // particle is far and alone, treated as an individial light.
				char str[15];
				sprintf(str, "lightPos[%d]", j);
				shader.SetVector3f(str, particle.Position);
				j++;
			}
			else { // Particle is near to the attraction, treated as a contributor of  the whole light.
				nCenter++;
			}
		}
	}

	shader.SetInteger("activeLightNum", j);
	shader.SetInteger("centerNum", nCenter);
	shader.SetVector3f("centerLightPos", camera.Position);
	shader.SetFloat("centerLightProperty.constant", 3.5f);
	shader.SetFloat("centerLightProperty.linear", 1.0f);
	shader.SetFloat("centerLightProperty.quadratic", 0.78);
	shader.SetVector3f("centerLightProperty.ambient", 0.000002f * nCenter, 0.000002f* nCenter, 0.000002f* nCenter); // 调参项： 一只萤火虫的光能对环境光有多大影响？
	shader.SetVector3f("centerLightProperty.diffuse", 219.0 / 255 * nCenter/2, 1.0f* nCenter/2, 47.0 / 255 * nCenter/2);
	shader.SetVector3f("centerLightProperty.specular", 130.0 / 255 * nCenter/3, 1.0f* nCenter/3, 47.0 / 255 * nCenter/3);

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

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}