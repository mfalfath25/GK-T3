#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shadowmapShader = BuildShader("multipleLight.vert", "multipleLight.frag", nullptr);
	BuildTexturedCube();
	BuildTexturedPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeEBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &planeEBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shadowmapShader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::vec3 cameraPos = glm::vec3(0, 2, 2);
	glm::vec3 cameraFront = glm::vec3(0, 0, 0);
	glm::mat4 view = glm::lookAt(cameraPos, cameraFront, glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shadowmapShader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attributes
	GLint viewPosLoc = glGetUniformLocation(this->shadowmapShader, "viewPos");
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

	// directional Light
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.direction"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.specular"), 0.1f, 0.1f, 0.1f);
	// point light 1 GREEN
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].position"), 5.0f, 5.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].ambient"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].specular"), 1.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].linear"), 0.09f); // distance = 50
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].quadratic"), 0.032f); // distance = 50
	// point light 2 GOLD
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].position"), -2.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].ambient"), 0.24f, 0.19f, 0.07f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].diffuse"), 0.75f, 0.6f, 0.22f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].specular"), 0.62f, 0.55f, 0.36f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].linear"), 0.09f); // distance = 50
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].quadratic"), 0.032f); // distance = 50
	// point light YELLOW
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].position"), 2.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].ambient"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].diffuse"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].specular"), 0.0f, 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].linear"), 0.7f); // distance = 7
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].quadratic"), 1.8f); // distance = 7
	// point light RED
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].position"), 0.0f, 3.0f, 2.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].ambient"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].diffuse"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].specular"), 0.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].linear"), 0.09f); // distance = 50
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].quadratic"), 0.032f); // distance = 50
	// spotLight
	glUniform3fv(glGetUniformLocation(this->shadowmapShader, "spotLight.position"), 1, &cameraPos[0]);
	glUniform3fv(glGetUniformLocation(this->shadowmapShader, "spotLight.direction"), 1, &cameraFront[0]);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.ambient"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.diffuse"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.specular"), 1.0f, 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
	
	DrawTexturedCube();
	DrawTexturedPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildTexturedCube()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &cube_texture);
	glBindTexture(GL_TEXTURE_2D, cube_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture);
	glBindTexture(GL_TEXTURE_2D, stexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("crate_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		
				// Bench base
		// Top base
		-0.5,  0.5, 0.5, 0, 0, 0.0f,  1.0f,  0.0f, // front top left 0
		0.5, 0.5, 0.5, 1, 0, 0.0f,  1.0f,  0.0f, // front top right 1
		0.5, 0.5, -0.5, 1, 1, 0.0f,  1.0f,  0.0f, // back top right 2
		-0.5,  0.5, -0.5, 0, 1, 0.0f,  1.0f,  0.0f, // back top left 3

		// Bottom base
		-0.5, 0.4, 0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // front bottom left 4
		0.5, 0.4, 0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // front bottom right 5
		0.5, 0.4, -0.5, 1, 1, 0.0f,  -1.0f,  0.0f, // back bottom right 6
		-0.5, 0.4, -0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // back bottom left 7

		// Front base
		-0.5, 0.4, 0.5, 0, 0, 0.0f,  0.0f,  1.0f, // front top left 8
		0.5, 0.4, 0.5, 1, 0, 0.0f,  0.0f,  1.0f, // front top right 9
		0.5, 0.5, 0.5, 1, 1, 0.0f,  0.0f,  1.0f, // back bottom right 10
		-0.5, 0.5, 0.5, 0, 1, 0.0f,  0.0f,  1.0f, // back bottom left 11

		// Back base
		-0.5, 0.4, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // front top left 12
		0.5, 0.4, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // front top right 13
		0.5, 0.5, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // back bottom right 14
		-0.5, 0.5, -0.5, 0, 1, 0.0f,  0.0f,  -1.0f, // back bottom left 15

		// Right base
		0.5, 0.4, 0.5, 0, 0, 1.0f,  0.0f,  0.0f, // front top left 16
		0.5, 0.4, -0.5, 1, 0, 1.0f,  0.0f,  0.0f, // front top right 17
		0.5, 0.5, -0.5, 1, 1, 1.0f,  0.0f,  0.0f, // back bottom right 18
		0.5, 0.5, 0.5, 0, 1, 1.0f,  0.0f,  0.0f, // back bottom left 19

		// Left base
		-0.5, 0.4, -0.5, 0, 0, -1.0f,  0.0f,  0.0f, // front top left 20
		-0.5, 0.4, 0.5, 1, 0, -1.0f,  0.0f,  0.0f, // front top right 21
		-0.5, 0.5, 0.5, 1, 1, -1.0f,  0.0f,  0.0f, // back bottom right 22
		-0.5, 0.5, -0.5, 0, 1, -1.0f,  0.0f,  0.0f, // back bottom left 23


		// Bench stand
		// Top stand
		-0.5, 0.8, -0.2, 0, 0, 0.0f,  1.0f,  0.0f, // front bottom left 24
		0.5, 0.8, -0.2, 1, 0, 0.0f,  1.0f,  0.0f, // front bottom right 25
		0.5, 0.8, -0.5, 1, 1, 0.0f,  1.0f,  0.0f, // back bottom right 26
		-0.5, 0.8, -0.5, 0, 1, 0.0f,  1.0f,  0.0f, // back bottom left 27

		// Bottom stand
		-0.5, 0.5, -0.2, 0, 0, 0.0f,  -1.0f,  0.0f, // front bottom left 28
		0.5, 0.5, -0.2, 1, 0, 0.0f,  -1.0f,  0.0f, // front bottom right 29
		0.5, 0.5, -0.5, 1, 1, 0.0f,  -1.0f,  0.0f, // back bottom right 30
		-0.5, 0.5, -0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // back bottom left 31

		// Front stand
		-0.5, 0.5, -0.2, 0, 0, 0.0f,  0.0f,  1.0f, // front bottom left 32
		0.5, 0.5, -0.2, 1, 0, 0.0f,  0.0f,  1.0f, // front bottom right 33
		0.5, 0.8, -0.2, 1, 1, 0.0f,  0.0f,  1.0f, // back bottom right 34
		-0.5, 0.8, -0.2, 0, 1, 0.0f,  0.0f,  1.0f, // back bottom left 35

		// Back stand
		-0.5, 0.5, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // front bottom left 36
		0.5, 0.5, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // front bottom right 37
		0.5, 0.8, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // back bottom right 38
		-0.5, 0.8, -0.5, 0, 1, 0.0f,  0.0f,  -1.0f, // back bottom left 39

		// Right stand
		0.5, 0.5, -0.2, 0, 0, 1.0f,  0.0f,  0.0f, // front bottom left 40
		0.5, 0.5, -0.5, 1, 0, 1.0f,  0.0f,  0.0f, // front bottom right 41
		0.5, 0.8, -0.5, 1, 1, 1.0f,  0.0f,  0.0f, // back bottom right 42
		0.5, 0.8, -0.2, 0, 1, 1.0f,  0.0f,  0.0f, // back bottom left 43

		// Left stand
		-0.5, 0.5, -0.5, 0, 0, -1.0f,  0.0f,  0.0f, // front bottom left 44
		-0.5, 0.5, -0.2, 1, 0, -1.0f,  0.0f,  0.0f, // front bottom right 45
		-0.5, 0.8, -0.2, 1, 1, -1.0f,  0.0f,  0.0f, // back bottom right 46
		-0.5, 0.8, -0.5, 0, 1, -1.0f,  0.0f,  0.0f, // back bottom left 47

		// Bench right leg
		// top right leg
		0.4, 0.4, 0.5, 0, 0, 0.0f,  1.0f,  0.0f, // front bottom left 48
		0.5, 0.4, 0.5, 1, 0, 0.0f,  1.0f,  0.0f, // front bottom right 49
		0.5, 0.4, -0.5, 1, 1,  0.0f,  1.0f,  0.0f, // back bottom right 50
		0.4, 0.4, -0.5, 0, 1, 0.0f,  1.0f,  0.0f, // back bottom left 51

		// bottom right leg
		0.4, 0.2, 0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // front bottom left 52
		0.5, 0.2, 0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // front bottom right 53
		0.5, 0.2, -0.5, 1, 1, 0.0f,  -1.0f,  0.0f, // back bottom right 54
		0.4, 0.2, -0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // back bottom left 55

		// Front right leg
		0.4, 0.2, 0.5, 0, 0, 0.0f,  0.0f,  1.0f, // front bottom left 56
		0.5, 0.2, 0.5, 1, 0, 0.0f,  0.0f,  1.0f, // front bottom right 57
		0.5, 0.4, 0.5, 1, 1, 0.0f,  0.0f,  1.0f, // back bottom right 58
		0.4, 0.4, 0.5, 0, 1, 0.0f,  0.0f,  1.0f, // back bottom left 59

		// Back right leg
		0.4, 0.2, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // front bottom left 60
		0.5, 0.2, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // front bottom right 61
		0.5, 0.4, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // back bottom right 62
		0.4, 0.4, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // back bottom left 63

		// Right right leg
		0.5, 0.2, 0.5, 0, 0, 1.0f, 0.0f, 0.0f, // front bottom left 64
		0.5, 0.2, -0.5, 1, 0, 1.0f, 0.0f, 0.0f, // front bottom right 65
		0.5, 0.4, -0.5, 1, 1, 1.0f, 0.0f, 0.0f, // back bottom right 66
		0.5, 0.4, 0.5, 0, 1, 1.0f, 0.0f, 0.0f, // back bottom left 67

		// Left right leg
		0.4, 0.2, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // front bottom left 68
		0.4, 0.2, 0.5, 1, 0, -1.0f, 0.0f, 0.0f, // front bottom right 69
		0.4, 0.4, 0.5, 1, 1, -1.0f, 0.0f, 0.0f, // back bottom right 70
		0.4, 0.4, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // back bottom left 71

		// Bench left leg
		// top left leg
		-0.5, 0.4, 0.5, 0, 0, 0.0f, 1.0f, 0.0f, // front bottom left 72
		-0.4, 0.4, 0.5, 1, 0, 0.0f, 1.0f, 0.0f, // front bottom right 73
		-0.4, 0.4, -0.5, 1, 1, 0.0f, 1.0f, 0.0f, // back bottom right 74
		-0.5, 0.4, -0.5, 0, 1, 0.0f, 1.0f, 0.0f, // back bottom left 75

		// bottom left leg
		-0.5, 0.2, 0.5, 0, 0, 0.0f, -1.0f, 0.0f, // front bottom left 76
		-0.4, 0.2, 0.5, 1, 0, 0.0f, -1.0f, 0.0f, // front bottom right 77
		-0.4, 0.2, -0.5, 1, 1, 0.0f, -1.0f, 0.0f, // back bottom right 78
		-0.5, 0.2, -0.5, 0, 1, 0.0f, -1.0f, 0.0f, // back bottom left 79

		// Front left leg
		-0.5, 0.2, 0.5, 0, 0, 0.0f, 0.0f, 1.0f, // front bottom left 80
		-0.4, 0.2, 0.5, 1, 0, 0.0f, 0.0f, 1.0f, // front bottom right 81
		-0.4, 0.4, 0.5, 1, 1, 0.0f, 0.0f, 1.0f, // back bottom right 82
		-0.5, 0.4, 0.5, 0, 1, 0.0f, 0.0f, 1.0f, // back bottom left 83

		// Back left leg
		-0.5, 0.2, -0.5, 0, 0, 0.0f, 0.0f, -1.0f, // front bottom left 84
		-0.4, 0.2, -0.5, 1, 0, 0.0f, 0.0f, -1.0f, // front bottom right 85
		-0.4, 0.4, -0.5, 1, 1, 0.0f, 0.0f, -1.0f, // back bottom right 86
		-0.5, 0.4, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // back bottom left 87

		// Right left leg
		-0.4, 0.2, 0.5, 0, 0, 1.0f, 0.0f, 0.0f, // front bottom left 88
		-0.4, 0.2, -0.5, 1, 0, 1.0f, 0.0f, 0.0f, // front bottom right 89
		-0.4, 0.4, -0.5, 1, 1, 1.0f, 0.0f, 0.0f, // back bottom right 90
		-0.4, 0.4, 0.5, 0, 1, 1.0f, 0.0f, 0.0f, // back bottom left 91

		// Left left leg
		-0.5, 0.2, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // front bottom left 92
		-0.5, 0.2, 0.5, 1, 0, -1.0f, 0.0f, 0.0f, // front bottom right 93
		-0.5, 0.4, 0.5, 1, 1, -1.0f, 0.0f, 0.0f, // back bottom right 94
		-0.5, 0.4, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // back bottom left 95

		// front
		//-0.5, -0.5, 0.5, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		//0.5, -0.5, 0.5, 1, 0,  0.0f,  0.0f,  1.0f, // 1
		//0.5,  0.5, 0.5, 1, 1,  0.0f,  0.0f,  1.0f, // 2
		//-0.5,  0.5, 0.5, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		 // right
		 //0.5,  0.5,  0.5, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 //0.5,  0.5, -0.5, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 //0.5, -0.5, -0.5, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 //0.5, -0.5,  0.5, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		//-0.5, -0.5, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		//0.5,  -0.5, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		//0.5,   0.5, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		//-0.5,  0.5, -0.5, 0, 1, 0.0f,  0.0f,  -1.0f, // 11

		 // left
		 //-0.5, -0.5, -0.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		 //-0.5, -0.5,  0.5, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		 //-0.5,  0.5,  0.5, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		 //-0.5,  0.5, -0.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper
		//0.5, 0.5,  0.5, 0, 0,   0.0f,  1.0f,  0.0f, // 16
		//-0.5, 0.5, 0.5, 1, 0,   0.0f,  1.0f,  0.0f, // 17
		//-0.5, 0.5, -0.5, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		//0.5, 0.5, -0.5, 0, 1,   0.0f,  1.0f,  0.0f, // 19

		// bottom
		//-0.5, -0.5, -0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		//0.5, -0.5, -0.5, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		//0.5, -0.5,  0.5, 1, 1,  0.0f,  -1.0f,  0.0f, // 22
		//-0.5, -0.5,  0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		// Bench base
		0, 1, 2, 0, 2, 3,  // top face
		4, 5, 6, 4, 6, 7,  // bottom face
		8, 9, 10, 8, 10, 11,  // front face
		12, 13, 14, 12, 14, 15,  // back face
		16, 17, 18, 16, 18, 19, // right face
		20, 21, 22, 20, 22, 23, // left face

		// Bench stand
		24, 25, 26, 24, 26, 27, // top face
		28, 29, 30, 28, 30, 31, // bottom face
		32, 33, 34, 32, 34, 35, // front face
		36, 37, 38, 36, 38, 39, // back face
		40, 41, 42, 40, 42, 43, // right face
		44, 45, 46, 44, 46, 47, // left face

		// Bench right leg
		48, 49, 50, 48, 50, 51, // top face
		52, 53, 54, 52, 54, 55, // bottom face
		56, 57, 58, 56, 58, 59, // front face
		60, 61, 62, 60, 62, 63, // back face
		64, 65, 66, 64, 66, 67, // right face
		68, 69, 70, 68, 70, 71, // left face

		// Bench left leg
		72, 73, 74, 72, 74, 75, // top face
		76, 77, 78, 76, 78, 79, // bottom face
		80, 81, 82, 80, 82, 83, // front face
		84, 85, 86, 84, 86, 87, // back face
		88, 89, 90, 88, 90, 91, // right face
		92, 93, 94, 92, 94, 95 // left face

		//0,  1,  2,  0,  2,  3,   // front
		//4,  5,  6,  4,  6,  7,   // right
		//8,  9,  10, 8,  10, 11,  // back
		//12, 14, 13, 12, 15, 14,  // left
		//16, 18, 17, 16, 19, 18,  // upper
		//20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildTexturedPlane()
{
	// Load and create a texture 
	glGenTextures(1, &plane_texture);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture2);
	glBindTexture(GL_TEXTURE_2D, stexture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("marble_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0, 0.0f,  1.0f,  0.0f,
		50.0, -0.5, -50.0, 50,  0, 0.0f,  1.0f,  0.0f,
		50.0, -0.5,  50.0, 50, 50, 0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0, 50, 0.0f,  1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedCube()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube_texture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, stexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);

	glBindVertexArray(cubeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTexturedPlane()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, stexture2);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.specular"), 3);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);

	glBindVertexArray(planeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Multiple Lighting Demo", 800, 600, false, false);
}