// Practica 2
// Miercoles 26 de agosto de 2026
// Gutierrez Benavides Frida Regina
// 320233454

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// ============================================================
		// CABEZA - CENTRO
		// ============================================================

		-0.5f,  0.2f, 0.0f,    0.20f, 0.80f, 1.00f,
		-0.5f, -0.75f, 0.0f,   0.20f, 0.80f, 1.00f,
		 0.6f, -0.75f, 0.0f,   0.20f, 0.80f, 1.00f,

		-0.5f,  0.2f, 0.0f,    0.20f, 0.80f, 1.00f,
		 0.6f, -0.75f, 0.0f,   0.20f, 0.80f, 1.00f,
		 0.6f,  0.2f, 0.0f,    0.20f, 0.80f, 1.00f,


		 // ============================================================
		 // LADO IZQUIERDO DE LA CABEZA
		 // ============================================================

		 -0.5f,  0.2f, 0.0f,    0.20f, 0.80f, 1.00f,
		 -0.5f, -0.75f, 0.0f,   0.20f, 0.80f, 1.00f,
		 -0.9f, -0.25f, 0.0f,   0.20f, 0.80f, 1.00f,


		 // ============================================================
		 // LADO DERECHO DE LA CABEZA
		 // ============================================================

		  0.6f,  0.2f, 0.0f,    0.20f, 0.80f, 1.00f,
		  0.6f, -0.75f, 0.0f,   0.20f, 0.80f, 1.00f,
		  1.0f, -0.25f, 0.0f,   0.20f, 0.80f, 1.00f,


		  // ============================================================
		  // OREJA IZQUIERDA
		  // ============================================================

		  -0.4f, 0.2f, 0.0f,     0.20f, 0.80f, 1.00f,
		  -0.4f, 0.9f, 0.0f,     0.20f, 0.80f, 1.00f,
		  -0.1f, 0.6f, 0.0f,     0.20f, 0.80f, 1.00f,

		  -0.4f, 0.2f, 0.0f,     0.20f, 0.80f, 1.00f,
		  -0.1f, 0.6f, 0.0f,     0.20f, 0.80f, 1.00f,
		  -0.1f, 0.2f, 0.0f,     0.20f, 0.80f, 1.00f,


		  // ============================================================
		  // OREJA DERECHA
		  // ============================================================

		   0.2f, 0.2f, 0.0f,     0.20f, 0.80f, 1.00f,
		   0.2f, 0.6f, 0.0f,     0.20f, 0.80f, 1.00f,
		   0.5f, 0.9f, 0.0f,     0.20f, 0.80f, 1.00f,

		   0.2f, 0.2f, 0.0f,     0.20f, 0.80f, 1.00f,
		   0.5f, 0.9f, 0.0f,     0.20f, 0.80f, 1.00f,
		   0.5f, 0.2f, 0.0f,     0.20f, 0.80f, 1.00f,


		   // ============================================================
		   // OJOS
		   // ============================================================

		   -0.20f, -0.22f, 0.0f,  0.0f, 0.0f, 0.0f,
			0.30f, -0.22f, 0.0f,  0.0f, 0.0f, 0.0f,


			// ============================================================
			// CACHETES
			// ============================================================

			-0.34f, -0.42f, 0.0f,  1.0f, 0.45f, 0.65f,
			 0.44f, -0.42f, 0.0f,  1.0f, 0.45f, 0.65f,


			 // ============================================================
			 // NARIZ
			 // ============================================================

			  0.05f, -0.27f, 0.0f,  1.0f, 0.35f, 0.50f,

			  // ============================================================
			  // BOCA
			  // ============================================================

			  // Linea vertical debajo de la nariz
			   0.05f, -0.31f, 0.0f,  0.0f, 0.0f, 0.0f,
			   0.05f, -0.40f, 0.0f,  0.0f, 0.0f, 0.0f,

			   // Parte izquierda de la sonrisa
				0.05f, -0.40f, 0.0f,  0.0f, 0.0f, 0.0f,
			   -0.02f, -0.45f, 0.0f,  0.0f, 0.0f, 0.0f,

			   -0.02f, -0.45f, 0.0f,  0.0f, 0.0f, 0.0f,
			   -0.11f, -0.42f, 0.0f,  0.0f, 0.0f, 0.0f,

			   // Parte derecha de la sonrisa
				0.05f, -0.40f, 0.0f,  0.0f, 0.0f, 0.0f,
				0.12f, -0.45f, 0.0f,  0.0f, 0.0f, 0.0f,

				0.12f, -0.45f, 0.0f,  0.0f, 0.0f, 0.0f,
				0.21f, -0.42f, 0.0f,  0.0f, 0.0f, 0.0f
	};

	unsigned int indices[] = {  // note that we start from 0!
		3,2,1,// second Triangle
		0,1,3,
		
	};

	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);

		// TAMANIO
	//glPointSize(10);
		// PUNTITO
	//glDrawArrays(GL_POINTS,0,4);

	// =======================================================================
		// CONEJO VERSION INICIAL
		// RAYITA/S
		//glDrawArrays(GL_LINES, 0, 4);

		// TRIANGULOS
		//glDrawArrays(GL_TRIANGLES, 4, 6); // El 4 es porque inicia del vértice 4, los dos anteriores son de las lineas

		// OREJAS
		//gldrawarrays(gl_lines, 10, 8);

		//gldrawarrays(gl_lines, 18, 4);

		// cara
		//glpointsize(15);

		//gldrawarrays(gl_points, 22, 1);
		//gldrawarrays(gl_points, 23, 1);

		//glpointsize(35);

		//gldrawarrays(gl_points, 24, 1);
		//gldrawarrays(gl_points, 25, 1);
	
		//glpointsize(12);

		//gldrawarrays(gl_points, 26, 1);
		//gldrawarrays(gl_points, 27, 1);

		// --------------------------------------------- --------------------------------------------- ---------------------------------------------
		// VERSION MEJORADA

		// CABEZA + OREJAS
		glDrawArrays(GL_TRIANGLES, 0, 24);

		// OJOS
		glPointSize(24);
		glDrawArrays(GL_POINTS, 24, 2);

		// CACHETES
		glPointSize(40);
		glDrawArrays(GL_POINTS, 26, 2);

		// NARIZ
		glPointSize(18);
		glDrawArrays(GL_POINTS, 28, 1);

		// BOCA
		glDrawArrays(GL_LINES, 29, 10);

		// =======================================================================
		
			// RAYITA/S
		//glDrawArrays(GL_LINES, 0, 4);

			// RAYITAS CERRADAS (CUADRITOS)
        //glDrawArrays(GL_LINE_LOOP,0,4);
        
			// TRIANGULO
        //glDrawArrays(GL_TRIANGLES,1,3);

			// USAR OTROS ORIGENES/ARREGLOS
        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}