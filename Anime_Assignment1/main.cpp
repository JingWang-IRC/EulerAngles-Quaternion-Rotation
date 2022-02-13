#define GLEW_STATIC
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>


#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
//#include "LightPoint.h"
//#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


float euler_y = 0.0;
float euler_p = 0.0;
float euler_r = 0.0;
int cameraMode = 1;
int rotateMode = 1;

#pragma region Model Data
float vertices_ground[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 100.0f,   // top right
	 0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 0.0f,   // bottom right
	 0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 100.0f    // top left 
};

unsigned int indices_ground[] = {
	0, 1, 2,
	2, 3, 0
};

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

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
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
#pragma endregion

#pragma region Light Declaration
LightDirectional lightD(glm::vec3(10.0f, 30.0f, 20.0f), glm::vec3(glm::radians(110.0f), glm::radians(30.0f), 0), glm::vec3(1.0f, 0.95f, 0.8f));
#pragma endregion

#pragma region Camera Declaration
// Initialize camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0, 15.0f, 20.0f), glm::radians(0.0f), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Texture Loading Function
unsigned int LoadTextureImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot)
{
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);

	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannel;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Texture image " << filename << " load failed." << endl;
	}
	stbi_image_free(data);

	//cout << filename << "Slot:  " << TexBuffer << endl;

	return TexBuffer;
}
#pragma endregion

#pragma region Load Sky Box

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "Cubemap tex succeeded to load at path: " << faces[i] << std::endl;
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

#pragma endregion

#pragma region Camera Operation Funtions
double previous_xPos, previous_yPos;
bool first_initialise_mouse = true;

void processInput_camera(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.SpeedZ = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.SpeedZ = -1.0;
	}
	else
	{
		camera.SpeedZ = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.SpeedX = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.SpeedX = -1.0;
	}
	else
	{
		camera.SpeedX = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.SpeedY = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.SpeedY = -1.0;
	}
	else
	{
		camera.SpeedY = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		euler_y -= 2.0f;
		/*if (euler_y <= 0)
			euler_y += 360;*/
	}
	else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		euler_y += 2.0f;
		/*if (euler_y >= 360)
			euler_y -= 360;*/
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		euler_p -= 2.0f;
		/*if (euler_p <= 0)
			euler_p += 360;*/
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		euler_p += 2.0f;
		/*if (euler_p >= 360)
			euler_p -= 360;*/
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		euler_r -= 2.0f;
		/*if (euler_r <= 0)
			euler_r += 360;*/
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		euler_r += 2.0f;
		/*if (euler_r >= 360)
			euler_r -= 360;*/
	}
}

void mouse_callback_camera(GLFWwindow* window, double xPos, double yPos)
{
	if (first_initialise_mouse == true)
	{
		previous_xPos = xPos;
		previous_yPos = yPos;
		first_initialise_mouse = false;
	}

	double Delta_x = xPos - previous_xPos;
	double Delta_y = yPos - previous_yPos;

	previous_xPos = xPos;
	previous_yPos = yPos;

	//cout << Delta_x << "   " << Delta_y << endl;
	camera.ProcessMouseMovement(Delta_x, Delta_y);
}
#pragma endregion

#pragma region Camera Switch Mode
void processInput_switchCameraMode(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)  // Third person view
	{
		cameraMode = 1;
		camera.Position = camera.LastPosition;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)  // First person view
	{
		cameraMode = 2;
		camera.LastPosition = camera.Position;

	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)  // Euler angle  mode
	{
		rotateMode = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)  // Quaternion mode
	{
		rotateMode = 2;
	}
}
#pragma endregion

#pragma region Shadow Map Debug
// renderQuad() renders a 1x1 XY quad in NDC
// This is used to debug shadow map 
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
#pragma endregion


int main(int argc, char* argv[])
{
	std::string exePath = argv[0];

#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1400, 1000, "My openGL game", NULL, NULL);

	if (window == NULL)
	{
		cout << "Open window failed." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "Init GlEW failed." << endl;
		glfwTerminate();
		return -1;
	}

	glfwSetCursorPosCallback(window, mouse_callback_camera);
	//glfwSetCursorPosCallback(window, mouse_callback_camera_ThirdPersonView);
	//glfwSetScrollCallback(window, scroll_callback_camera_ThirdPersonView);

	//glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion

#pragma region Init Shader Program
	Shader* shader_ground = new Shader("VertexShader_ground.vert", "FragmentShader_ground.frag");
	Shader* shader_blinnPhong = new Shader("VertexShader_blinnPhong.vert", "FragmentShader_blinnPhong.frag");
	Shader* shader_skybox = new Shader("VertexShader_skybox.vert", "FragmentShader_skybox.frag");
	Shader* shader_shadowMap = new Shader("VertexShader_shadowMap.vert", "FragmentShader_shadowMap.frag");
	Shader* shader_shadowDebug = new Shader("VertexShader_shadowDebug.vert", "FragmentShader_shadowDebug.frag");
#pragma endregion 

#pragma region Pass light to shaders;
	shader_ground->use();
	shader_ground->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_ground->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_ground->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));

	shader_blinnPhong->use();
	shader_blinnPhong->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_blinnPhong->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_blinnPhong->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));
#pragma endregion

#pragma region Load Models

	Model plane(exePath.substr(0, exePath.find_last_of('\\')) + "\\plane\\plane.obj");
	Model rightPropeller(exePath.substr(0, exePath.find_last_of('\\')) + "\\plane\\rightPropeller.obj");
	Model leftPropeller(exePath.substr(0, exePath.find_last_of('\\')) + "\\plane\\leftPropeller.obj");
	Model frontPropeller(exePath.substr(0, exePath.find_last_of('\\')) + "\\plane\\frontPropeller.obj");

	// cube VAO VBO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int VBO_ground;
	glGenBuffers(1, &VBO_ground);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ground), vertices_ground, GL_STATIC_DRAW);

	unsigned int VAO_ground;
	glGenVertexArrays(1, &VAO_ground);
	glBindVertexArray(VAO_ground);

	unsigned int EBO_ground;
	glGenBuffers(1, &EBO_ground);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_ground), indices_ground, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
#pragma endregion

#pragma region Init and Load Texture;
	stbi_set_flip_vertically_on_load(true);
	unsigned int TexBuffer_ground;
	TexBuffer_ground = LoadTextureImageToGPU("ground.jpg", GL_RGB, GL_RGB, 0);
	//unsigned int TexBufferB;
	//TexBufferB = LoadTextureImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion

#pragma region Init FBO for Shadow Map;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 2 * 4096, SHADOW_HEIGHT = 2 * 4096;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion

#pragma region Init Skybox Texture;
	vector<std::string> faces
	{
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\posx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\negx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\posy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\negy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\posz.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\negz.jpg"
	};
	stbi_set_flip_vertically_on_load(false);
	unsigned int cubemapTexture = loadCubemap(faces);
#pragma endregion

#pragma region Prepare MVP matrices
	glm::mat4 trans;
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 1400.0f / 1000.0f, 0.1f, 300.0f);
#pragma endregion

	glm::quat quaternion = glm::angleAxis(glm::radians(0.0f), glm::vec3(1., 0., 0.));
	float old_eulerY = euler_y;
	float old_eulerR = euler_r;
	float old_eulerP = euler_p;
	float delta_p = 0;
	float delta_y = 0;
	float delta_r = 0;

	float startTime = glfwGetTime();

	// Start drawing
	while (!glfwWindowShouldClose(window))
	{
		// Mouse Action
		processInput_camera(window);

		// Camera Mode
		processInput_switchCameraMode(window);

		float currentTime = glfwGetTime();

#pragma region Create Shadow Map

		float near_plane = 0.0f, far_plane = 100.5f;
		glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
		// Set the light position following the character, and add an offset to draw shadows in distance (some may be cut under the limitaion of depth map)
		glm::mat4 lightView = glm::lookAt(lightD.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		shader_shadowMap->use();
		shader_shadowMap->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		//glDisable(GL_CULL_FACE);

		// rotation mode 1, use euler angles
		if (rotateMode == 1)
		{
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			trans = glm::rotate(trans, glm::radians(euler_r), glm::vec3(0.0f, 0.0f, 1.0f));
			trans = glm::rotate(trans, glm::radians(euler_y), glm::vec3(0.0f, 1.0f, 0.0f));
			trans = glm::rotate(trans, glm::radians(euler_p), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		// rotation mode 2, use quaternion
		else if (rotateMode == 2)
		{
			delta_p = euler_p - old_eulerP;
			delta_y = euler_y - old_eulerY;
			delta_r = euler_r - old_eulerR;

			glm::vec3 rotationAxisX = glm::vec3(glm::toMat4(quaternion) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			glm::quat rot1 = glm::angleAxis(glm::radians(delta_p), rotationAxisX);
			quaternion = rot1 * quaternion;

			glm::vec3 rotationAxisY = glm::vec3(glm::toMat4(quaternion) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			glm::quat rot2 = glm::angleAxis(glm::radians(delta_y), rotationAxisY);
			quaternion = rot2 * quaternion;

			glm::vec3 rotationAxisZ = glm::vec3(glm::toMat4(quaternion) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			glm::quat rot3 = glm::angleAxis(glm::radians(delta_r), rotationAxisZ);
			quaternion = rot3 * quaternion;

			old_eulerP = euler_p;
			old_eulerY = euler_y;
			old_eulerR = euler_r;

			trans = glm::toMat4(quaternion);
		}
		
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 18.0f, 0.0f));
		modelMat = modelMat * trans;
		shader_shadowMap->SetMatrix("model", modelMat);

		plane.Draw(shader_shadowMap);

		// update first person view camera
		camera.firstPersonPosition = glm::mat3(trans) * glm::vec3(0.0f, 2.0f, -1.0f) + glm::vec3(0.0f, 18.0f, 0.0f);
		camera.firstPersonTarget = glm::mat3(trans) * glm::vec3(0.0f, 2.0f, 1.0f) + glm::vec3(0.0f, 18.0f, 0.0f);
		camera.firstPersonUp = glm::mat3(trans) * glm::vec3(0.0f, 1.0f, 0.0f);

		// draw the propellers
		glm::mat4 trans_leftPropeller = glm::translate(trans, glm::vec3(4.5f, 0.095f, 1.85f));
		trans_leftPropeller = glm::rotate(trans_leftPropeller, glm::radians(360.0f * (currentTime - startTime)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans_leftPropeller = glm::translate(trans_leftPropeller, glm::vec3(-4.5f, -0.095f, -1.85f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 18.0f, 0.0f));
		modelMat = modelMat * trans_leftPropeller;
		shader_shadowMap->SetMatrix("model", modelMat);
		leftPropeller.Draw(shader_shadowMap);

		glm::mat4 trans_rightPropeller = glm::translate(trans, glm::vec3(-4.5f, 0.095f, 1.85f));
		trans_rightPropeller = glm::rotate(trans_rightPropeller, glm::radians(360.0f * (currentTime - startTime)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans_rightPropeller = glm::translate(trans_rightPropeller, glm::vec3(4.5f, -0.095f, -1.85f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 18.0f, 0.0f));
		modelMat = modelMat * trans_rightPropeller;
		shader_shadowMap->SetMatrix("model", modelMat);
		rightPropeller.Draw(shader_shadowMap);

		glm::mat4 trans_frontPropeller = glm::translate(trans, glm::vec3(0.0f, 0.16f, 7.14f));
		trans_frontPropeller = glm::rotate(trans_frontPropeller, glm::radians(360.0f * (currentTime - startTime)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans_frontPropeller = glm::translate(trans_frontPropeller, glm::vec3(0.0f, 0.16f, -7.14f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 18.0f, 0.0f));
		modelMat = modelMat * trans_frontPropeller;
		shader_shadowMap->SetMatrix("model", modelMat);
		frontPropeller.Draw(shader_shadowMap);

		//glCullFace(GL_BACK);

		// draw the ground
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
		modelMat = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
		shader_shadowMap->SetMatrix("model", modelMat);

		glBindVertexArray(VAO_ground);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
		glDrawElements(GL_TRIANGLES, sizeof(indices_ground), GL_UNSIGNED_INT, 0);


		//glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// Clear Screen
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, 1400, 1000);

		if (cameraMode == 1)
			viewMat = camera.GetViewMatrix();
		else if (cameraMode == 2)
			viewMat = camera.GetViewMatrixFirstPerson();

		//Shadow map debugger
		/*shader_shadowDebug->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader_shadowDebug->SetUniform1i("depthMap", 0);
		renderQuad();*/

#pragma endregion

#pragma region Draw ground
		// Ground
		shader_ground->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		shader_ground->SetMatrix("transform", trans);
		shader_ground->SetMatrix("modelMat", modelMat);
		shader_ground->SetMatrix("viewMat", viewMat);
		shader_ground->SetMatrix("projMat", projMat);
		shader_ground->SetUniform3f("viewPos", camera.Position);
		shader_ground->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_ground);
		shader_ground->SetUniform1i("groundTexture_diffuse", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader_ground->SetUniform1i("shadowMap", 1);
		//glUniform1i(glGetUniformLocation(shader_ground->ID, "shadowMap"), 1);

		glBindVertexArray(VAO_ground);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
		glDrawElements(GL_TRIANGLES, sizeof(indices_ground), GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region Draw plane

		if (rotateMode == 1)
		{
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			trans = glm::rotate(trans, glm::radians(euler_r), glm::vec3(0.0f, 0.0f, 1.0f));
			trans = glm::rotate(trans, glm::radians(euler_y), glm::vec3(0.0f, 1.0f, 0.0f));
			trans = glm::rotate(trans, glm::radians(euler_p), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateMode == 2)
		{
			trans = glm::toMat4(quaternion);
		}

		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 18.0f, 0.0f));
		shader_blinnPhong->SetMatrix("transform", trans);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		shader_blinnPhong->SetMatrix("viewMat", viewMat);
		shader_blinnPhong->SetMatrix("projMat", projMat);
		shader_blinnPhong->SetUniform3f("viewPos", camera.Position);
		shader_blinnPhong->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader_blinnPhong->SetUniform1i("shadowMap", 1);

		plane.Draw(shader_blinnPhong);

		trans_leftPropeller = glm::translate(trans, glm::vec3(4.5f, 0.095f, 1.85f));
		trans_leftPropeller = glm::rotate(trans_leftPropeller, glm::radians(360.0f * (currentTime - startTime)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans_leftPropeller = glm::translate(trans_leftPropeller, glm::vec3(-4.5f, -0.095f, -1.85f));
		shader_blinnPhong->SetMatrix("transform", trans_leftPropeller);
		leftPropeller.Draw(shader_blinnPhong);

		trans_rightPropeller = glm::translate(trans, glm::vec3(-4.5f, 0.095f, 1.85f));
		trans_rightPropeller = glm::rotate(trans_rightPropeller, glm::radians(360.0f * (currentTime - startTime)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans_rightPropeller = glm::translate(trans_rightPropeller, glm::vec3(4.5f, -0.095f, -1.85f));
		shader_blinnPhong->SetMatrix("transform", trans_rightPropeller);
		rightPropeller.Draw(shader_blinnPhong);

		trans_frontPropeller = glm::translate(trans, glm::vec3(0.0f, 0.16f, 7.14f));
		trans_frontPropeller = glm::rotate(trans_frontPropeller, glm::radians(360.0f * (currentTime - startTime)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans_frontPropeller = glm::translate(trans_frontPropeller, glm::vec3(0.0f, 0.16f, -7.14f));
		shader_blinnPhong->SetMatrix("transform", trans_frontPropeller);
		frontPropeller.Draw(shader_blinnPhong);

#pragma endregion

#pragma region Draw Skybox

		glDepthFunc(GL_LEQUAL);
		shader_skybox->use();
		viewMat = glm::mat4(glm::mat3(viewMat)); // remove translation from the view matrix
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.04f, 0.0f));

		shader_skybox->SetMatrix("modelMat", modelMat);
		shader_skybox->SetMatrix("viewMat", viewMat);
		shader_skybox->SetMatrix("projMat", projMat);
		shader_skybox->SetUniform1i("skybox", 0);

		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

#pragma endregion

		// Clean up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPos();
	}


}
