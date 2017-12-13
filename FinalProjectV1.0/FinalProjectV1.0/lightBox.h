#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "resource_manager.h"
#include "Camera.h"

class LightBox {

public:
	LightBox() {};
	void Init(GLuint width, GLuint height);
	void  Draw(Shader &shader, Camera &camera);
	void Update();

private:
	glm::vec3 lightPos;
	GLuint VBO, lightVAO;
	GLuint Width, Height;
};