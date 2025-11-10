//Práctica 12
//Sandra Laparra Miranda
//Fecha de entrega: 9 de noviembre de 2025
//Número de cuenta: 311243563

#include <iostream>
#include <cmath>
#include <fstream>  //Para guardar y cargar de archivos 

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void saveFramesToFile(const char* filename); //Guardar en formato .txt animación precargada
void loadFramesFromFile(const char* filename); 

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;

float fLegL = 0.0f; // Front Left
float fLegR = 0.0f; // Front Right
float bLegL = 0.0f; // Back Left
float bLegR = 0.0f; // Back Right
float head = 0.0f;
float tail = 0.0f;



//KeyFrames
float dogPosX, dogPosY, dogPosZ;
float dogRotX = 0.0f; //para inclinar el cuerpo

#define MAX_FRAMES 9
int i_max_steps = 600;
int i_curr_steps = 0;
typedef struct _frame {

	//Posición y rotación del cuerpo
	float rotDog;		//Rotación en Y (girar)
	float rotDogInc;
	float dogRotX;		//Rotación en X (inclinar)
	float dogRotXInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;

	//Partes individuales
	float head;
	float headInc;
	float tail;
	float tailInc;
	float fLegL;
	float fLegLInc;
	float fLegR;
	float fLegRInc;
	float bLegL;
	float bLegLInc;
	float bLegR;
	float bLegRInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].dogRotX = dogRotX;
	KeyFrame[FrameIndex].head = head;

	KeyFrame[FrameIndex].tail = tail;
	KeyFrame[FrameIndex].fLegL = fLegL;
	KeyFrame[FrameIndex].fLegR = fLegR;
	KeyFrame[FrameIndex].bLegL = bLegL;
	KeyFrame[FrameIndex].bLegR = bLegR;

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;
	rotDog = KeyFrame[0].rotDog;
	dogRotX = KeyFrame[0].dogRotX;

	tail = KeyFrame[0].tail;
	fLegL = KeyFrame[0].fLegL;
	fLegR = KeyFrame[0].fLegR;
	bLegL = KeyFrame[0].bLegL;
	bLegR = KeyFrame[0].bLegR;
}
void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
	KeyFrame[playIndex].dogRotXInc = (KeyFrame[playIndex + 1].dogRotX - KeyFrame[playIndex].dogRotX) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;

	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].fLegLInc = (KeyFrame[playIndex + 1].fLegL - KeyFrame[playIndex].fLegL) / i_max_steps;
	KeyFrame[playIndex].fLegRInc = (KeyFrame[playIndex + 1].fLegR - KeyFrame[playIndex].fLegR) / i_max_steps;
	KeyFrame[playIndex].bLegLInc = (KeyFrame[playIndex + 1].bLegL - KeyFrame[playIndex].bLegL) / i_max_steps;
	KeyFrame[playIndex].bLegRInc = (KeyFrame[playIndex + 1].bLegR - KeyFrame[playIndex].bLegR) / i_max_steps;
}


void saveFramesToFile(const char* filename)
{
	//Abrir un archivo de salida 
	std::ofstream outFile(filename);

	if (!outFile.is_open())
	{
		std::cout << "Error: No se pudo abrir el archivo " << filename << " para escribir." << std::endl;
		return;
	}

	//Escribir el número de frames
	outFile << FrameIndex << std::endl;

	//Recorrer y escribir los datos de cada keyframe
	for (int i = 0; i < FrameIndex; i++)
	{
		outFile << KeyFrame[i].dogPosX << " ";
		outFile << KeyFrame[i].dogPosY << " ";
		outFile << KeyFrame[i].dogPosZ << " ";
		outFile << KeyFrame[i].rotDog << " ";
		outFile << KeyFrame[i].dogRotX << " ";
		outFile << KeyFrame[i].head << " ";
		outFile << KeyFrame[i].tail << " ";
		outFile << KeyFrame[i].fLegL << " ";
		outFile << KeyFrame[i].fLegR << " ";
		outFile << KeyFrame[i].bLegL << " ";
		outFile << KeyFrame[i].bLegR << std::endl;
	}

	//Cerrar el archivo
	outFile.close();
	std::cout << "¡Keyframes guardados exitosamente en " << filename << "!" << std::endl;
}



void loadFramesFromFile(const char* filename)
{
	//Abrir un archivo de entrada 
	std::ifstream inFile(filename);

	if (!inFile.is_open())
	{
		std::cout << "Error: No se pudo abrir el archivo " << filename << " para leer." << std::endl;
		return;
	}

	//Leer el número de frames
	int framesToLoad = 0;
	inFile >> framesToLoad;

	if (framesToLoad > MAX_FRAMES)
	{
		std::cout << "Error: El archivo tiene más frames (" << framesToLoad
			<< ") que el máximo permitido (" << MAX_FRAMES << ")" << std::endl;
		inFile.close();
		return;
	}

	//Recorrer y leer los datos de cada keyframe
	for (int i = 0; i < framesToLoad; i++)
	{
		
		inFile >> KeyFrame[i].dogPosX;
		inFile >> KeyFrame[i].dogPosY;
		inFile >> KeyFrame[i].dogPosZ;
		inFile >> KeyFrame[i].rotDog;
		inFile >> KeyFrame[i].dogRotX;
		inFile >> KeyFrame[i].head;
		inFile >> KeyFrame[i].tail;
		inFile >> KeyFrame[i].fLegL;
		inFile >> KeyFrame[i].fLegR;
		inFile >> KeyFrame[i].bLegL;
		inFile >> KeyFrame[i].bLegR;
	}

	//Actualizar el contador de frames
	FrameIndex = framesToLoad;

	//Cerrar el archivo
	inFile.close();

	//Resetear la posición del perro al nuevo Frame 0
	resetElements();

	std::cout << "¡Keyframes cargados exitosamente desde " << filename << "!" << std::endl;
}



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 12 Sandra Laparra", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].dogRotX = 0;
		KeyFrame[i].dogRotXInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].fLegL = 0;
		KeyFrame[i].fLegLInc = 0;
		KeyFrame[i].fLegR = 0;
		KeyFrame[i].fLegRInc = 0;
		KeyFrame[i].bLegL = 0;
		KeyFrame[i].bLegLInc = 0;
		KeyFrame[i].bLegR = 0;
		KeyFrame[i].bLegRInc = 0;
	}

	//Frame 1: Sentarse
	KeyFrame[1].dogPosY = -0.12f;	//Baja el cuerpo
	KeyFrame[1].dogRotX = -30.0f;	//Inclina el cuerpo hacia atrás 

	KeyFrame[1].bLegL = -90.0f;		//Dobla pata trasera izq. hacia adelante
	KeyFrame[1].bLegR = 90.0f;		//Dobla pata trasera der. hacia adelante

	KeyFrame[1].fLegL = -30.0f;
	KeyFrame[1].fLegR = -30.0f;

	KeyFrame[1].head = 0.0f;
	KeyFrame[1].tail = -10.0f;

	//Frame 2: Mantener sentado
	KeyFrame[2] = KeyFrame[1];
	KeyFrame[2].tail = 10.0f; //Mueve la cola

	//Frame 3: Levantar pata para saludar
	KeyFrame[3] = KeyFrame[2];
	KeyFrame[3].fLegR = 70.0f;  //Levanta Pata Derecha 
	KeyFrame[3].head = 0.0f;
	KeyFrame[3].tail = -10.0f;

	//Frame 4: Mantener pata levantada
	KeyFrame[4] = KeyFrame[3];
	KeyFrame[4].tail = 10.0f; //Mueve la cola

	//Frame 5: Bajar pata
	KeyFrame[5] = KeyFrame[2]; //Vuelve a la posición sentada fLegR = -30.0f
	KeyFrame[5].tail = -10.0f;

	//Frame 6: Levantarse
	KeyFrame[6] = KeyFrame[0]; //Vuelve a la posición inicial
	KeyFrame[6].tail = 20.0f;

	//Frame 7: Mantener de pie
	KeyFrame[7] = KeyFrame[0]; //Mantiene posición inicial

	FrameIndex = 8;


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		//Body
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(dogRotX), glm::vec3(1.0f, 0.0f, 0.0f)); // <-- *** NUEVA ROTACIÓN APLICADA ***
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(fLegL), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(fLegR), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(bLegL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(bLegR), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls

	if (keys[GLFW_KEY_4])
	{
		head += 0.1f;
	}

	if (keys[GLFW_KEY_5])
	{
		head -= 0.1f;
	}

	if (keys[GLFW_KEY_2])
	{
		rotDog += 1.0f;
	}

	if (keys[GLFW_KEY_3])
	{
		rotDog -= 1.0f;
	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.001;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.001;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.001;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.001;
	}


	if (keys[GLFW_KEY_Z]) //Cola
	{
		tail += 1.0f;
	}
	if (keys[GLFW_KEY_X])
	{
		tail -= 1.0f;
	}
	if (keys[GLFW_KEY_C]) //Pata Delantera Izquierda
	{
		fLegL += 1.0f;
	}
	if (keys[GLFW_KEY_V])
	{
		fLegL -= 1.0f;
	}
	if (keys[GLFW_KEY_B]) //Pata Delantera Derecha
	{
		fLegR += 1.0f;
	}
	if (keys[GLFW_KEY_N])
	{
		fLegR -= 1.0f;
	}
	if (keys[GLFW_KEY_M]) //Pata Trasera Izquierda
	{
		bLegL += 1.0f;
	}
	if (keys[GLFW_KEY_COMMA])
	{
		bLegL -= 1.0f;
	}
	if (keys[GLFW_KEY_PERIOD]) //Pata Trasera Derecha
	{
		bLegR += 1.0f;
	}
	if (keys[GLFW_KEY_SLASH])
	{
		bLegR -= 1.0f;
	}



	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	
	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//Manejar eventos de una sola pulsación (PRESS)
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_L) //Play/Stop
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				interpolation(); //Primera interpolación
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			}
			else
			{
				play = false;
			}
		}

		if (key == GLFW_KEY_K) // Guardar frame en memoria
		{
			if (FrameIndex < MAX_FRAMES)
			{
				saveFrame();
			}
		}

		if (key == GLFW_KEY_P) // Guardar frames en archivo
		{
			saveFramesToFile("keyframes.txt");
		}

		//Bloque que implementa la carga de archivo llamado keyframes.txt
		if (key == GLFW_KEY_O)
		{
			loadFramesFromFile("keyframes.txt");
		}


		if (key == GLFW_KEY_SPACE)
		{
			active = !active;
			if (active)
				Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			else
				Light1 = glm::vec3(0);
		}

		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	// Manejar estado de teclas (PRESS/RELEASE) para movimiento continuo
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action ==
			GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}



void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].headInc;
			rotDog += KeyFrame[playIndex].rotDogInc;
			dogRotX += KeyFrame[playIndex].dogRotXInc;


			tail += KeyFrame[playIndex].tailInc;
			fLegL += KeyFrame[playIndex].fLegLInc;
			fLegR += KeyFrame[playIndex].fLegRInc;
			bLegL += KeyFrame[playIndex].bLegLInc;
			bLegR += KeyFrame[playIndex].bLegRInc;


			i_curr_steps++;
		}

	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
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