// Gutiérrez Benavides Frida Regina
// 320233454
// Practica 10
// Viernes 23 de octubre de 2026

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>

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

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.80f, 4.2f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 2.0f, 0.0f),
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
// Perro rigido persiguiendo un globo. Mismos OBJ y shaders.
bool AnimBall = true;
double tiempoAnim = 0.0;
glm::vec3 posPerro(0.0f), posPelota(0.0f);
glm::vec3 centroPerro(0.0f), centroPelota(0.0f, 0.31f, 0.47f);
float giroPerro = 90.0f;
float inclinacionPerro = 0.0f;
float balanceoPerro = 0.0f;
glm::vec3 pivoteInclinacion(0.0f, 0.25f, 0.0f);
glm::vec3 limiteMinPerro(-0.2f, 0.0f, -0.3f);
glm::vec3 limiteMaxPerro(0.2f, 0.6f, 0.3f);
float giroPelota = 0.0f;
float radioGlobo = 0.15f;
float alturaNariz = 0.31f;
float avanceNariz = 0.30f;
float radioRecorridoGlobo = 1.1f;
float desfaseNariz = 0.0f;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// PARAMETROS PARA AJUSTAR LA ANIMACION.
const double DURACION_CICLO = 10.0;
const float PI_ANIM = 3.14159265359f;
const float RADIO_PERRO = 1.05f;
const float ALTURA_SALTO = 0.34f;
const float DURACION_SALTO = 1.20f;
const float OSCILACION_GLOBO = 0.50f; // Diferencia entre altura maxima y minima.
const float RETRASO_PERRO = 0.38f; // Radianes. Mantener entre 0 y 1.
// Correcciones opcionales de la nariz, en coordenadas del modelo original.
const float AJUSTE_NARIZ_Y = 0.0f;
const float AJUSTE_NARIZ_Z = 0.0f;

// Salto de TODO el perro. Parabola: despega, llega al maximo y aterriza.
float SaltoParabolico(float t, float instanteMaximo)
{
	float u = (t - instanteMaximo) / (DURACION_SALTO * 0.5f);
	if (u <= -1.0f || u >= 1.0f) return 0.0f;
	return ALTURA_SALTO * (1.0f - u * u);
}

// Trayectoria propia del globo: circulo en XZ y oscilacion vertical.
glm::vec3 RecorridoGlobo(float t)
{
	float a = 2.0f * PI_ANIM * t / static_cast<float>(DURACION_CICLO);
	float c = std::cos(a);
	float alturaMinima = alturaNariz + AJUSTE_NARIZ_Y + ALTURA_SALTO + radioGlobo;
	float y = alturaMinima + OSCILACION_GLOBO * c * c;
	return glm::vec3(radioRecorridoGlobo * std::cos(a), y,
		radioRecorridoGlobo * std::sin(a));
}

// Trayectoria propia del perro. No usa la posicion calculada del globo.
glm::vec3 RecorridoPerro(float t, float& anguloGrados)
{
	float a = 2.0f * PI_ANIM * t / static_cast<float>(DURACION_CICLO);
	float c = std::cos(a);
	// Recupera la distancia al acercarse cada punto bajo del globo.
	float retraso = RETRASO_PERRO * c * c;
	float fase = a - desfaseNariz - retraso;
	anguloGrados = -fase * 180.0f / PI_ANIM;
	float salto1 = SaltoParabolico(t, static_cast<float>(DURACION_CICLO * 0.25));
	float salto2 = SaltoParabolico(t, static_cast<float>(DURACION_CICLO * 0.75));
	return glm::vec3(RADIO_PERRO * std::cos(fase), salto1 + salto2,
		RADIO_PERRO * std::sin(fase));
}

// Movimiento alegre: rotaciones de TODO el perro, sin deformarlo.
const float BALANCEO_GRADOS = 4.0f;
const float INCLINACION_SALTO_GRADOS = 10.0f;
const float INCLINACION_CAMINAR_GRADOS = 2.0f;

// Misma rotacion local que se aplica al dibujar: primero Z y despues X.
glm::vec3 RotarInclinacion(glm::vec3 p)
{
	float x = inclinacionPerro * PI_ANIM / 180.0f;
	float z = balanceoPerro * PI_ANIM / 180.0f;
	p.x -= pivoteInclinacion.x;
	p.y -= pivoteInclinacion.y;
	p.z -= pivoteInclinacion.z;
	float px = std::cos(z) * p.x - std::sin(z) * p.y;
	float py = std::sin(z) * p.x + std::cos(z) * p.y;
	return glm::vec3(px + pivoteInclinacion.x,
		std::cos(x) * py - std::sin(x) * p.z + pivoteInclinacion.y,
		std::sin(x) * py + std::cos(x) * p.z + pivoteInclinacion.z);
}

void ActualizarInclinacion(float t)
{
	float a = 2.0f * PI_ANIM * t / static_cast<float>(DURACION_CICLO);
	float fraccionSalto = ALTURA_SALTO > 0.0f ? posPerro.y / ALTURA_SALTO : 0.0f;
	balanceoPerro = BALANCEO_GRADOS * std::sin(8.0f * a) * (1.0f - 0.7f * fraccionSalto);
	inclinacionPerro = -INCLINACION_CAMINAR_GRADOS * std::sin(8.0f * a)
		* (1.0f - fraccionSalto);
	const float maximos[] = { static_cast<float>(DURACION_CICLO * 0.25),
							 static_cast<float>(DURACION_CICLO * 0.75) };
	for (int i = 0; i < 2; ++i)
	{
		float u = (t - maximos[i]) / (DURACION_SALTO * 0.5f);
		if (u > -1.0f && u < 1.0f)
			// Nariz hacia arriba al subir y hacia abajo al aterrizar.
			// En el punto de contacto vuelve a estar horizontal.
			inclinacionPerro += INCLINACION_SALTO_GRADOS * std::sin(PI_ANIM * u) * u * u;
	}

	// Compensacion conservadora del piso usando las ocho esquinas del modelo.
	// Evita que el balanceo hunda las patas. En pleno salto no suele hacer falta.
	float minimoY = 1.0e20f;
	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 p((i & 1) ? limiteMaxPerro.x : limiteMinPerro.x,
			(i & 2) ? limiteMaxPerro.y : limiteMinPerro.y,
			(i & 4) ? limiteMaxPerro.z : limiteMinPerro.z);
		float y = RotarInclinacion(p).y;
		if (y < minimoY) minimoY = y;
	}
	if (posPerro.y + minimoY < 0.0f)
		posPerro.y = -minimoY;
}

void Animation()
{
	if (AnimBall)
		tiempoAnim = std::fmod(tiempoAnim + deltaTime, DURACION_CICLO);
	float t = static_cast<float>(tiempoAnim);
	posPelota = RecorridoGlobo(t); // Se conserva ball.obj como esfera flotante.
	posPerro = RecorridoPerro(t, giroPerro);
	ActualizarInclinacion(t);
	// Una vuelta suave del globo por ciclo, alrededor de su propio centro.
	giroPelota = 360.0f * t / static_cast<float>(DURACION_CICLO);
}


// Consulta las dimensiones sin editar ningun OBJ.
struct LimitesOBJ { float minX, minY, minZ, maxX, maxY, maxZ; };
bool LeerLimites(const char* ruta, LimitesOBJ& b)
{
	std::ifstream archivo(ruta);
	if (!archivo) return false;
	b = { 1e20f, 1e20f, 1e20f, -1e20f, -1e20f, -1e20f };
	std::string linea, tipo;
	bool encontrado = false;
	while (std::getline(archivo, linea))
	{
		std::istringstream datos(linea);
		datos.imbue(std::locale::classic());
		float x, y, z;
		if ((datos >> tipo) && tipo == "v" && (datos >> x >> y >> z))
		{
			if (x < b.minX) b.minX = x;
			if (y < b.minY) b.minY = y;
			if (z < b.minZ) b.minZ = z;
			if (x > b.maxX) b.maxX = x;
			if (y > b.maxY) b.maxY = y;
			if (z > b.maxZ) b.maxZ = z;
			encontrado = true;
		}
	}
	return encontrado;
}

// Estima la nariz en el extremo delantero (+Z) del modelo.
// Solo consulta vertices del archivo; no modifica la geometria.
bool EstimarNariz(const char* ruta, const LimitesOBJ& b, glm::vec3& nariz)
{
	std::ifstream archivo(ruta);
	if (!archivo) return false;
	std::string linea, tipo;
	float sumaX = 0.0f, sumaY = 0.0f, sumaZ = 0.0f;
	int cantidad = 0;
	float margen = (b.maxZ - b.minZ) * 0.002f;
	while (std::getline(archivo, linea))
	{
		std::istringstream datos(linea);
		datos.imbue(std::locale::classic());
		float x, y, z;
		if ((datos >> tipo) && tipo == "v" && (datos >> x >> y >> z)
			&& z >= b.maxZ - margen)
		{
			sumaX += x;
			sumaY += y;
			sumaZ += z;
			++cantidad;
		}
	}
	if (!cantidad) return false;
	nariz = glm::vec3(sumaX / cantidad, sumaY / cantidad, sumaZ / cantidad);
	return true;
}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Perro alegre y globo - saltos y balanceo Gutiérrez Benavides Frida Regina", nullptr, nullptr);

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

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model Dog((char*)"Models/RedDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");

	// RedDog mira hacia +Z en su orientacion original.
	// Estos centros evitan que los modelos orbiten al girarlos.
	LimitesOBJ perro, pelota;
	if (!LeerLimites("Models/RedDog.obj", perro) ||
		!LeerLimites("Models/ball.obj", pelota))
	{
		std::cerr << "No se pudieron leer los OBJ. Revisa la carpeta Models." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glm::vec3 nariz;
	if (!EstimarNariz("Models/RedDog.obj", perro, nariz))
	{
		std::cerr << "No se pudo estimar el punto de la nariz." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	// La referencia del perro queda en el piso, debajo de su eje delantero.
	centroPerro = glm::vec3(nariz.x, perro.minY,
		(perro.minZ + perro.maxZ) * 0.5f);
	centroPelota = glm::vec3((pelota.minX + pelota.maxX) * 0.5f,
		(pelota.minY + pelota.maxY) * 0.5f,
		(pelota.minZ + pelota.maxZ) * 0.5f);
	radioGlobo = (pelota.maxY - pelota.minY) * 0.5f;
	alturaNariz = nariz.y - centroPerro.y;
	avanceNariz = nariz.z - centroPerro.z + AJUSTE_NARIZ_Z;
	limiteMinPerro = glm::vec3(perro.minX - centroPerro.x, 0.0f,
		perro.minZ - centroPerro.z);
	limiteMaxPerro = glm::vec3(perro.maxX - centroPerro.x,
		perro.maxY - perro.minY, perro.maxZ - centroPerro.z);
	pivoteInclinacion = glm::vec3(0.0f, (perro.maxY - perro.minY) * 0.45f, 0.0f);
	// La nariz describe un circulo algo mayor que el centro del cuerpo.
	// En los dos puntos bajos, ese circulo coincide con el del globo.
	radioRecorridoGlobo = std::sqrt(RADIO_PERRO * RADIO_PERRO + avanceNariz * avanceNariz);
	desfaseNariz = std::atan2(avanceNariz, RADIO_PERRO);

	// Evalua la pose inicial sin avanzar el reloj.
	deltaTime = 0.0f;
	Animation();




	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
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

	// La carga inicial no forma parte de los 10 segundos.
	lastFrame = static_cast<GLfloat>(glfwGetTime());

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



		// PISO: espacio para el circuito completo.
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, 2.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		// PERRO RIGIDO: solo traslacion y rotacion de TODO el modelo.
		model = glm::translate(glm::mat4(1.0f), posPerro);
		model = glm::rotate(model, glm::radians(giroPerro), glm::vec3(0, 1, 0));
		model = glm::translate(model, pivoteInclinacion);
		model = glm::rotate(model, glm::radians(inclinacionPerro), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(balanceoPerro), glm::vec3(0, 0, 1));
		model = glm::translate(model, -pivoteInclinacion);
		model = glm::translate(model, -centroPerro);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Dog.Draw(lightingShader);

		// GLOBO: oscilacion vertical y circulo independientes del perro.
		model = glm::translate(glm::mat4(1.0f), posPelota);
		model = glm::rotate(model, glm::radians(giroPelota), glm::vec3(0, 1, 0));
		model = glm::translate(model, -centroPelota);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
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
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		tiempoAnim = 0.0;
		deltaTime = 0.0f;
		AnimBall = true;
		Animation();
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		AnimBall = !AnimBall;

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