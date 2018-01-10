#include "environment.h"

GLuint loadCubemap(vector<const GLchar*> faces);

void Environment::Init(GLuint width, GLuint height, ParticleGenerator *ps, GLchar *path) {
	Width = width;
	Height = height;
	envModel = new Model((GLchar *)path);
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
	faces.push_back("skyboxes/lowpolynight/bk.jpg");
	faces.push_back("skyboxes/lowpolynight/ft.jpg");
	faces.push_back("skyboxes/lowpolynight/up.jpg");
	faces.push_back("skyboxes/lowpolynight/dn.jpg");
	faces.push_back("skyboxes/lowpolynight/rt.jpg");
	faces.push_back("skyboxes/lowpolynight/lf.jpg");
	cubemapTexture = loadCubemap(faces);

	vector<const GLchar*> faces_real;
	faces_real.push_back("skyboxes/night/ft.jpg");
	faces_real.push_back("skyboxes/night/bk.jpg");
	faces_real.push_back("skyboxes/night/up.jpg");
	faces_real.push_back("skyboxes/night/dn.jpg");
	faces_real.push_back("skyboxes/night/rt.jpg");
	faces_real.push_back("skyboxes/night/lf.jpg");
	cubemapTexture_real = loadCubemap(faces_real);

	ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.frag", nullptr, "skyboxShader");
	skyboxShader = ResourceManager::GetShader("skyboxShader");
}

void Environment::Update() {

}

void Environment::Draw(Shader &shader, Camera &camera, bool real) {

	// 绘制单独的萤火虫的光照
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
	shader.SetFloat("exposure", 0.3);


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


	// 绘制等效光照点的光照
	shader.SetInteger("activeLightNum", j);
	shader.SetInteger("centerNum", nCenter);
	shader.SetVector3f("centerLightPos", camera.Position);
	shader.SetFloat("centerLightProperty.constant", 3.5f);
	shader.SetFloat("centerLightProperty.linear", 1.0f);
	shader.SetFloat("centerLightProperty.quadratic", 0.78);
	shader.SetVector3f("centerLightProperty.ambient", 0.000002f * nCenter, 0.000002f* nCenter, 0.000002f* nCenter);
	shader.SetVector3f("centerLightProperty.diffuse", 219.0 / 255 * nCenter / 1.5, 1.0f* nCenter / 1.5, 47.0 / 255 * nCenter / 1.5);
	shader.SetVector3f("centerLightProperty.specular", 130.0 / 255 * nCenter / 2, 1.0f* nCenter / 2, 47.0 / 255 * nCenter / 2);

	glm::mat4 view(1.0f);
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);

	glm::mat4 model(1.0f);
	model = glm::translate(model, terrainPos);
	model = glm::scale(model, terrainScale);
	shader.SetMatrix4("model", model);

	envModel->Draw(shader);

	// Draw skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader.Use();
	glm::mat4 view_skybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection_skybox = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	skyboxShader.SetMatrix4("view", view_skybox);
	skyboxShader.SetMatrix4("projection", projection_skybox);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	skyboxShader.SetInteger("skybox", 0);
	if (!real) { // different mode, different skyboxes
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_real);
	}
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

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