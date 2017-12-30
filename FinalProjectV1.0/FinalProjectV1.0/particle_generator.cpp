#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount, GLuint width, GLuint height)
	: shader(shader), texture(texture), amount(amount), Width(width), Height(height)
{
	this->init();
	this->newParticleCounter = 0;
}

void ParticleGenerator::SetEmitPos(glm::vec3 emitPos1, glm::vec3 emitPos2, glm::vec3 emitPos3) {
	emitPoses.push_back(emitPos1);
	emitPoses.push_back(emitPos2);
	emitPoses.push_back(emitPos3);
}

void ParticleGenerator::init()
{
	GLfloat vertices[] = {
		//顶点位置数据	      法向量数据
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// Only position attribute needed
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());

}

void ParticleGenerator::Update(GLfloat dt, GLfloat newParticles, glm::vec3 dstPos)
{
	dt /= 6;
	newParticleCounter += newParticles;
	if (newParticleCounter >= 1) {
		// Add new particles 
		for (GLuint i = 0; i < newParticleCounter; ++i)
		{
			int unusedParticle = this->firstUnusedParticle();
			this->respawnParticle(this->particles[unusedParticle]);
			if (liveAmount < amount) {
				liveAmount++;
			}
			char tmp[100];
			sprintf(tmp, "liveAmount: %d", liveAmount);
			//std::cout << tmp << endl;
		}
	}
	newParticleCounter -= (int)newParticleCounter;
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		// reduce life
		p.Life -= dt; 
		if (p.Life > 0.0f)
		{	// particle is alive, thus update

			// if need, change the accelerate
			p.VelocityChangeCounter++;
			if (p.VelocityChangeCounter > 100) {
				float randX = (rand() % 100 - 50) / 5.0f;
				float randY = (rand() % 100 - 50) / 5.0f;
				float randZ = (rand() % 100 - 50) / 5.0f;
				p.Accelerate = { randX, randY, randZ };
				p.VelocityChangeCounter = 0;
			}

			// calculate distance between current particle and chasing point
			float distance = sqrt((dstPos[0] - p.Position[0])*(dstPos[0] - p.Position[0])
				+ (dstPos[1] - p.Position[1]) *  (dstPos[1] - p.Position[1])
				+ (dstPos[2] - p.Position[2]) * (dstPos[2] - p.Position[2]));

			p.distance = distance;

			p.randVelocity += p.Accelerate * dt;
			if (distance <= 1.5) { // When the particle is close enough to the attraction  这里的 2.5 代表吸引区大小（密集程度）
				p.Velocity  = p.randVelocity;
			}else{
				float constraintVal = rand() % 40 + 3; // 对每个个体随机约束程度
				glm::vec3 V_center = constraintVal*(dstPos - p.Position) / distance;
				p.Velocity = p.randVelocity + V_center;
			}
			// calculate V_center
			//float distanceParam = 0.0001 * distance;   //线性正相关
			//float distanceParam = exp(3-distance);  //指数负相关
			p.Position += p.Velocity * dt;
			//TODO: use sin function to disturb color and light
			if (p.Life < dt) {  // The particle nearly dead (will died in next frame)
				this->liveAmount--;
				char tmp[100];
				sprintf(tmp, "liveAmount: %d", liveAmount);
				//std::cout << tmp << endl;
			}
		}
	}
}

void ParticleGenerator::respawnParticle(Particle &particle)
{
	GLfloat random1 = ((rand() % 100) - 50) / 100.0f; // [-5, 5]
	GLfloat random2 = ((rand() % 100) - 50) / 100.0f;
	GLfloat random3 = ((rand() % 100) - 50) / 100.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	glm::vec3 initPosDisturb = glm::vec3(random3*5, random2*5, random1*5);
	particle.Position = emitPoses[rand()%3] + initPosDisturb;  // 位置扰动
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 50.0f;
	particle.is_prime = false;
	particle.mode = 0;
	particle.Velocity = glm::vec3(random1, random2, random3);
	particle.VelocityChangeCounter = 0;
	particle.Accelerate = glm::vec3(0.0f);
	particle.randVelocity = glm::vec3(0.0f);

	// Set up sound engin
	particle.soundEngine = irrklang::createIrrKlangDevice();
	particle.music = particle.soundEngine->play3D("sounds/insect.wav", irrklang::vec3df(0, 0, 0), true, false, true);
	if (particle.music)
		particle.music->setMinDistance(0.5f);
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

inline irrklang::vec3df posFormatTrans(glm::vec3 src) {
	irrklang::vec3df dst(src[0], src[1], src[2]);
	return dst;
}

void ParticleGenerator::Draw(Camera &camera) {

	glm::mat4 view(1.0f);
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	this->shader.Use();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, particle.Position);
			model = glm::scale(model, glm::vec3(0.02f)); // Make it a smaller cube
			shader.SetMatrix4("model", model);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			// Update the location of camera and each particle for the sound engine
			particle.soundEngine->setListenerPosition(posFormatTrans(camera.Position), posFormatTrans(-camera.Front));
			if (particle.music) {
				particle.music->setPosition(posFormatTrans(particle.Position));
			}
		}
	}

	
}


GLuint ParticleGenerator::GetLiveNum() {
	return this->liveAmount;
}

glm::vec3 ParticleGenerator::getIthLivePos(int i) {
	int j = 0;
	for (Particle particle : this->particles) {
		if (particle.Life > 0.0f) {
			if (j == i) {
				return particle.Position;
			}
			j++;
		}
	}
}