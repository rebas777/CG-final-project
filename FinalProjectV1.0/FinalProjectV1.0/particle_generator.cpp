#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Shader bugShader, Texture2D texture, GLuint amount, GLuint width, GLuint height)
	: shader(shader), bugShader(bugShader), texture(texture), amount(amount), Width(width), Height(height)
{
	this->amount_son = 5000;
	this->dying = false;
	this->isReal = false;
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

	float vertices1[] = {
		-0.5, -1, 0, 0.33, 0,
		0.5, -1, 0, 0.66, 0,
		1, -0.5, 0, 1, 0.33,

		-0.5, -1, 0, 0.33, 0,
		1, -0.5, 0, 1, 0.33,
		1, 0.5, 0.0, 1, 0.66,

		-0.5, -1, 0, 0.33, 0,
		1, 0.5, 0.0, 1, 0.66,
		0.5, 1, 0, 0.66, 1,

		-0.5, -1, 0, 0.33, 0,
		0.5, 1, 0, 0.66, 1,
		-0.5, 1, 0, 0.33, 1,

		-0.5, -1, 0, 0.33, 0,
		-0.5, 1, 0, 0.33, 1,
		-1, 0.5, 0, 0, 0.66,

		-0.5, -1, 0, 0.33, 0,
		-1, 0.5, 0, 0, 0.66,
		-1, -0.5, 0, 0, 0.33
	};

	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
															  // front face
															  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
															  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
															  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
															  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
															  -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
															  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
																													// left face
																													-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
																													-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
																													-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
																													-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
																													-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
																													-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
																																										  // right face
																																										  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
																																										  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
																																										  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
																																										  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
																																										  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
																																										  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
																																																							   // bottom face
																																																							   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
																																																							   1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
																																																							   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
																																																							   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
																																																							   -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
																																																							   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
																																																																					 // top face
																																																																					 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
																																																																					 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
																																																																					 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
																																																																					 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
																																																																					 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
																																																																					 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());

	for (GLuint i = 0; i < this->amount_son; i++) {
		this->sonParticles.push_back(Particle());
	}

	// Initialize bug model
	bugModel = new Model("models/insect/insect.obj");

}

void ParticleGenerator::Update(GLfloat dt, GLfloat newParticles, glm::vec3 dstPos)
{ // 有约束的自由运动
	dt /= 5;
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

			if (p.Life < dt) {  // The particle nearly dead (will died in next frame)
				this->liveAmount--;
				delete(p.soundEngine);
				delete(p.music);
			}
		}
	}
}

void ParticleGenerator::RandUpdate(GLfloat dt, GLfloat newParticles) 
{// 无约束的自由运动
	dt /= 5;
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


			p.randVelocity += p.Accelerate * dt;
			p.Velocity = p.randVelocity;
			p.Position += p.Velocity * dt;
			//TODO: use sin function to disturb color and light
			if (p.Life < dt) {  // The particle nearly dead (will died in next frame)
				this->liveAmount--;
				//delete(p.soundEngine);
				//delete(p.music);
			}
		}
	}
}

void ParticleGenerator::DieUpdate(GLfloat dt) {


	if (dying == false) {
		for (GLuint i = 0; i < this->amount; i++) {
			Particle &p = this->particles[i];
			if (p.Life > 0.0f) {
				p.Life = 10.0f;
				p.Accelerate = glm::vec3(0.0, -0.5, 0.0);
				p.Velocity /= 18;
			}
		}
	}
	dying = true;

    // emit 1 sonParticles from every living parentParticle & update parent particles
	for (GLuint i = 0; i < this->amount; i++) {
		Particle &pp = this->particles[i];
		// reduce life
		pp.Life -= dt;
		if (pp.Life > 0.0) {
			// update parent particle color.
			pp.Color -= (dt / pp.Life)*pp.Color;

			for (int j = 0; j < 1; j++) {
				int unusedParticle = this->firstUnusedParticle_son();
				this->respawnParticle_son(this->sonParticles[unusedParticle], i);
			}
			pp.Velocity += pp.Accelerate * dt;
			pp.Position += pp.Velocity * dt;

			if (pp.Life < dt) {  // The particle nearly dead (will died in next frame)
				//delete(pp.soundEngine);
				//delete(pp.music);
			}
		}
	}
	// update son particles
	for (GLuint i = 0; i < amount_son; i++) {
		Particle &sp = this->sonParticles[i];
		if (sp.Life > 0) {
			sp.Life -= dt;
			glm::vec3 gravity = glm::vec3(0, -2.0, 0);
			sp.Velocity += gravity * dt;
			sp.Position += sp.Velocity *dt;
		}
	}
}

void ParticleGenerator::ResetSystem() {
	for (int i = 0; i < amount; i++) {
		Particle &pp = this->particles[i];
		pp.Life = 0;
	}
	for (int j = 0; j < amount_son; j++) {
		Particle &sp = this->sonParticles[j];
		sp.Life = 0;
	}
}


void ParticleGenerator::respawnParticle(Particle &particle)
{
	GLfloat random1 = ((rand() % 100) - 50) / 100.0f; // [-5, 5]
	GLfloat random2 = ((rand() % 100) - 50) / 100.0f;
	GLfloat random3 = ((rand() % 100) - 50) / 100.0f;
	glm::vec3 initPosDisturb = glm::vec3(random3*5, random2*5, random1*5);
	particle.Position = emitPoses[rand()%3] + initPosDisturb;  // 位置扰动
	particle.Color = glm::vec3(0.3f, 1.0f, 0.0f);
	particle.Life = 500.0f;
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
void ParticleGenerator::respawnParticle_son(Particle &particle, int parentNum) {
	GLfloat random1 = ((rand() % 100) - 50) / 100.0f; // [-5, 5]
	GLfloat random2 = ((rand() % 100) - 50) / 100.0f;
	GLfloat random3 = ((rand() % 100) - 50) / 100.0f;
	particle.Position = this->particles[parentNum].Position;
	particle.Color = this->particles[parentNum].Color;
	particle.Life = 0.8;
	particle.Velocity = glm::vec3(random1, random2, random3);
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

GLuint lastUsedParticle_son = 0;
GLuint ParticleGenerator::firstUnusedParticle_son() {
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle_son; i < this->amount_son; ++i) {
		if (this->sonParticles[i].Life <= 0.0f) {
			lastUsedParticle_son = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle_son; ++i) {
		if (this->sonParticles[i].Life <= 0.0f) {
			lastUsedParticle_son = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle_son = 0;
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
	bugShader.Use();
	bugShader.SetMatrix4("view", view);
	bugShader.SetMatrix4("projection", projection);
	for (Particle particle : this->particles) // draw all parent particles
	{
		if (particle.Life > 0.0f)
		{
			if (isReal) {
				//shader.Use();
				//shader.SetVector3f("lightColor", particle.Color);
				//glm::mat4 model(1.0f);
				glm::mat4 bugmodel(1.0f);
				//model = glm::translate(model, particle.Position);

				
				bugmodel = glm::translate(bugmodel, particle.Position);
				float x = particle.Velocity.x;
				if (x == 0)
					return;
				float z = particle.Velocity.z;
				float raw_theta = atan(abs(z / x));
				float theta;
				if (x > 0) {
					if (z >= 0) {
						theta = raw_theta;
					}
					else {
						theta = 2 * PI - raw_theta;
					}
				}
				else { // x < 0
					if (z >= 0) {
						theta = PI - raw_theta;
					}
					else {
						theta = PI + raw_theta;
					}
				}
				bugmodel = glm::rotate(bugmodel, (float)PI/2, glm::vec3(0.0f, 1.0f, 0.0f));// 矫正模型偏差
				bugmodel = glm::rotate(bugmodel, theta, glm::vec3(0.0f, 1.0f, 0.0f));
				bugmodel = glm::translate(bugmodel, glm::vec3(0.0, 0.0, 0.10)); // 矫正模型偏差
				
				bugmodel = glm::scale(bugmodel, glm::vec3(0.0006f)); // Make it a smaller cube
				
				//model = glm::scale(model, glm::vec3(0.02f));
				/*shader.Use();
				shader.SetMatrix4("model", model);
				shader.SetVector3f("lightColor", particle.Color);
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);*/
				bugShader.Use();
				bugShader.SetMatrix4("model", bugmodel);
				bugShader.SetVector3f("lightColor", particle.Color);
				bugModel->Draw(bugShader);
			}
			else {
				shader.Use();
				shader.SetVector3f("lightColor", particle.Color);
				glm::mat4 model(1.0f);
				model = glm::translate(model, particle.Position);
				model = glm::scale(model, glm::vec3(0.02f)); // Make it a smaller cube
				shader.SetMatrix4("model", model);
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
			}

			// Update the location of camera and each particle for the sound engine
			particle.soundEngine->setListenerPosition(posFormatTrans(camera.Position), posFormatTrans(-camera.Front));
			if (particle.music) {
				particle.music->setPosition(posFormatTrans(particle.Position));
			}
		}
	}

	if (dying) {
		for (Particle sonParticle : this->sonParticles) {
			if (sonParticle.Life > 0) {
				shader.Use();
				shader.SetVector3f("lightColor", sonParticle.Color);
				glm::mat4 model(1.0f);
				model = glm::translate(model, sonParticle.Position);
				model = glm::scale(model, glm::vec3(0.004f)); // Make it a smaller cube
				shader.SetMatrix4("model", model);
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
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