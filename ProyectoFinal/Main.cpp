/*
	===============================================================================
	Archivo: Main.cpp
	Propósito: Simulación del ordenamiento de los toroides de las Torres de Hanoy

	Autor: Ramirez Garcia Diego Andres
	Fecha de creación: 06 diciembre 2023
	===============================================================================
*/

/*
  ___  _            _   _
 |   \(_)_ _ ___ __| |_(_)_ _____ ___
 | |) | | '_/ -_) _|  _| \ V / -_|_-<
 |___/|_|_| \___\__|\__|_|\_/\___/__/

*/
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/stb_image.h"
#include "SOIL2/SOIL2.h"
#include "Headers/Shader.h"
#include "Headers/Camera.h"
#include "Headers/Model.h"

/* 
  ___         _       _
 | _ \_ _ ___| |_ ___| |_ _  _ _ __  ___ ___
 |  _/ '_/ _ \  _/ _ \  _| || | '_ \/ -_|_-<
 |_| |_| \___/\__\___/\__|\_, | .__/\___/__/
						  |__/|_|
*/
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();

/* 
 __   __               __   ___
 \ \ / /_ _ _ _ ___   / /  / __|___ _ _  ___
  \ V / _` | '_(_-<  / /  | (__/ _ \ ' \(_-<
   \_/\__,_|_| /__/ /_/    \___\___/_||_/__/

*/ 
const GLuint WIDTH = 1500, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Camera  camera(glm::vec3(2.2f, 1.2f, 4.3f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

glm::vec3 lightPos(5.106f, 0.0f, 17.425f);

bool repeat = false;
int flag = 0;
bool anim = false;
bool step1 = true;
bool step2 = false;
bool step3 = false;
bool step4 = false;
bool step5 = false;
bool step6 = false;
bool step7 = false;
glm::vec3 mov(0.0f, 0.0f, 0.0f);
glm::vec3 mov2(0.0f, 0.0f, 0.0f);
glm::vec3 mov3(0.0f, 0.0f, 0.0f);

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
/*
  __  __   _   ___ _  _
 |  \/  | /_\ |_ _| \| |
 | |\/| |/ _ \ | || .` |
 |_|  |_/_/ \_\___|_|\_|

*/
int main(){
	// --------------> GLFW
	glfwInit();
	// Set all the required options for GLFW: OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Torres de Hanoy - Ramirez Garcia Diego Andres", nullptr, nullptr);
	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	// Settings for working with OpenGL context within a GLFW window
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// --------------> GLEW 
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	/*
		Create Shaders
	*/
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");	// iluminación (ambient, point, spot)


	/*
		Models 
	*/
	Model plano((char*)"Models/Plano/Plano.obj");
	Model torres((char*)"Models/Torres/Torres.obj");
	Model toroide1((char*)"Models/Toroides/Toroide1.obj");
	Model toroide2((char*)"Models/Toroides/Toroide2.obj");
	Model toroide3((char*)"Models/Toroides/Toroide3.obj");

	/*
		VAO and VBO
	*/

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		animacion();
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// OpenGL options
		glEnable(GL_DEPTH_TEST);
		// Local variables
		glm::mat4 view;
		glm::mat4 model(1);
		/*
			Draw
		*/
		lightingShader.Use();

		// Set material properties
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// Setting View
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Setting directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, 0.0f, -0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

		// Get the uniform locations of Model, View and Projection 
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		// Setting view with camera coords
		view = camera.GetViewMatrix();
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Definition of model matrix to draw
		


		// ======================================================================================
		// ======================================================================================
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTranspacencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plano.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		torres.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(mov.x, mov.y, mov.z));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		toroide3.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(mov2.x, mov2.y, mov2.z));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		toroide2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(mov3.x, mov3.y, mov3.z));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		toroide1.Draw(lightingShader);
		// ======================================================================================
		// ======================================================================================


		// unlink VAO
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
/*
  ___             _   _
 | __|  _ _ _  __| |_(_)___ _ _  ___
 | _| || | ' \/ _|  _| / _ \ ' \(_-<
 |_| \_,_|_||_\__|\__|_\___/_||_/__/
*/

// Moves/alters the camera positions based on user input
void DoMovement() { 
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}
// To animation
void animacion() {
	if (anim)
	{
		if (step1)
		{
			if (flag == 0) {
				mov.y += 1.2f * deltaTime;
				if (mov.y > 0.72)
					flag = 1;
			}
			if (flag == 1) {
				mov.x += 1.2f * deltaTime;
				if (mov.x > 2.72)
					flag = 2;
			}
			if (flag == 2) {
				mov.y -= 1.2f * deltaTime;
				if (mov.y < -0.63f) {
					step1 = false;
					step2 = true;
					flag = 0;
				}
			}

		}
		if (step2)
		{
			if (flag == 0) {
				mov2.y += 1.2f * deltaTime;
				if (mov2.y > 1.048)
					flag = 1;
			}
			if (flag == 1) {
				mov2.x += 1.2f * deltaTime;
				if (mov2.x > 1.37)
					flag = 2;
			}
			if (flag == 2) {
				mov2.y -= 1.2f * deltaTime;
				if (mov2.y < -0.347f) {
					step2 = false;
					step3 = true;
					flag = 0;
				}
			}

		}
		if (step3) {
			if (flag == 0) {
				mov.y += 1.2f * deltaTime;
				if (mov.y > 0.754)
					flag = 1;
			}
			if (flag == 1) {
				mov.x -= 1.2f * deltaTime;
				if (mov.x < 1.375)
					flag = 2;
			}
			if (flag == 2) {
				mov.y -= 1.2f * deltaTime;
				if (mov.y < -0.335f) {
					step3 = false;
					step4 = true;
					flag = 0;
				}
			}
		}
		if (step4) {
			if (flag == 0) {
				mov3.y += 1.2f * deltaTime;
				if (mov3.y > 1.425)
					flag = 1;
			}
			if (flag == 1) {
				mov3.x += 1.2f * deltaTime;
				if (mov3.x > 2.74)
					flag = 2;
			}
			if (flag == 2) {
				mov3.y -= 1.2f * deltaTime;
				if (mov3.y < -0.007f) {
					step4 = false;
					step5 = true;
					flag = 0;
				}
			}
		}
		if (step5) {
			if (flag == 0) {
				mov.y += 1.2f * deltaTime;
				if (mov.y > 0.75)
					flag = 1;
			}
			if (flag == 1) {
				mov.x -= 1.2f * deltaTime;
				if (mov.x < 0.02)
					flag = 2;
			}
			if (flag == 2) {
				mov.y -= 1.2f * deltaTime;
				if (mov.y < -0.635) {
					step5 = false;
					step6 = true;
					flag = 0;
				}
			}
		}
		if (step6) {
			if (flag == 0) {
				mov2.y += 1.2f * deltaTime;
				if (mov2.y > 1.075)
					flag = 1;
			}
			if (flag == 1) {
				mov2.x += 1.2f * deltaTime;
				if (mov2.x > 2.732)
					flag = 2;
			}
			if (flag == 2) {
				mov2.y -= 1.2f * deltaTime;
				if (mov2.y < -0.015) {
					step6 = false;
					step7 = true;
					flag = 0;
				}
			}
		}
		if (step7) {
			if (flag == 0) {
				mov.y += 1.2f * deltaTime;
				if (mov.y > 0.82)
					flag = 1;
			}
			if (flag == 1) {
				mov.x += 1.2f * deltaTime;
				if (mov.x > 2.73)
					flag = 2;
			}
			if (flag == 2) {
				mov.y -= 1.2f * deltaTime;
				if (mov.y < -0.01) {
					step7 = false;
					step1 = true;
					anim = false;
					repeat = true;
					flag = 0;
				}
			}
		}
	}
}
// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE]){
		if (repeat) {
			mov = glm::vec3(0.0f) ;
			mov2 = glm::vec3(0.0f);
			mov3 = glm::vec3(0.0f);
		}
		anim = true;
	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
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