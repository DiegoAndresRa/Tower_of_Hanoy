#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "Headers/stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Headers/Shader.h"
#include "Headers/Camera.h"
#include "Headers/Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(5.106f, 0.0f, 17.425f);
//glm::vec3 spot_posicion(5.0f, 2.5f, -5.0f);
//glm::vec3 spot_direccion(5.0f, -10.0f, -5.0f);
bool active;
// Animación


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Torres de Hanoy - Ramirez Garcia Diego Andres", nullptr, nullptr);

	if (nullptr == window){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set this to true so GLEW knows to use a modern approach to  
	// retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	/*
		Create Shaders
	*/
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");	// iluminación (ambient, point, spot)
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag"); // figuras básicas

	/*
		Models (OBJ)
	*/

	Model torres((char*)"Models/Torres/Torres.obj");
	Model toroide1((char*)"Models/Toroides/Toroide1.obj");
	Model toroide2((char*)"Models/Toroides/Toroide2.obj");
	Model toroide3((char*)"Models/Toroides/Toroide3.obj");

	/*
		VAO and VBO
	*/
	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Set texture units
	lightingShader.Use();

	/*
		Perspective Matrix
	*/
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	/*
		Main loop
	*/
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		/*
			Draw
		*/
		lightingShader.Use();

		// Set material properties
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 20.0f);
		// Setting View
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Setting directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f,0.0f,-0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

		// Setting view with camera coords
		glm::mat4 view;
		view = camera.GetViewMatrix();
		// Get the uniform locations of Model, View and Projection 
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Definition of model matrix to draw
		glm::mat4 model(1);
		view = camera.GetViewMatrix();

		// ======> link VAO
		model = glm::mat4(1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTranspacencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		torres.Draw(lightingShader); ;
		toroide1.Draw(lightingShader);
		toroide2.Draw(lightingShader);
		toroide3.Draw(lightingShader);


		// ======> unlink
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement(){
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]){
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]){
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]){
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS){
			keys[key] = true;
		}else if (action == GLFW_RELEASE){
			keys[key] = false;
		}
	}

	//if (keys[GLFW_KEY_SPACE])
	//{
	//	active = !active;
	//	if (active)
	//	{
	//		Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
	//	}
	//	else
	//	{
	//		Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
	//		Light2 = glm::vec3(0);
	//		Light3 = glm::vec3(0);
	//		Light4 = glm::vec3(0);

	//	}
	//}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos){
	if (firstMouse){
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}