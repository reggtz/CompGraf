// Gutierrez Benavides Frida Regina
// 320233454
// Previo 5
// Domingo 13 de septiembre de 2026

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);

const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f,
codo = 0.0f,
muneca = 0.0f,
dedo11 = 0.0f,
dedo12 = 0.0f,
dedo13 = 0.0f,
dedo21 = 0.0f,
dedo22 = 0.0f,
dedo23 = 0.0f,
dedo31 = 0.0f,
dedo32 = 0.0f,
dedo33 = 0.0f,
dedo41 = 0.0f,
dedo42 = 0.0f,
dedo43 = 0.0f,
pulgar1 = 0.0f,
pulgar2 = 0.0f,
pulgar3 = 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico Gutierrez Benavides Frida Regina", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
		
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
					
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp

		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	
		glBindVertexArray(VAO);
		
		// Model bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // A

		// Model antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // B

		// Model palma
		model = glm::translate(modelTemp, glm::vec3(1.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 = modelTemp = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // C

		// ==========================================================
		// DEDO 1
		// ==========================================================

		// Rectangulo 1
		model = glm::translate(modelTemp2, glm::vec3(-0.75f, 0.36f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo11), glm::vec3(0.0f, 0.0f, 1.0f));

		// Guardamos la articulacion
		modelTemp = model;

		// Movemos al centro del rectangulo
		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 2
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo12), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 3
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo13), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// ==========================================================
		// DEDO 2
		// ==========================================================

		// Rectangulo 1
		model = glm::translate(modelTemp2, glm::vec3(-0.75f, 0.12f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo21), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 2
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo22), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 3
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo23), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// ==========================================================
		// DEDO 3
		// ==========================================================

		// Rectangulo 1
		model = glm::translate(modelTemp2, glm::vec3(-0.75f, -0.12f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo31), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 2
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo32), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 3
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo33), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// ==========================================================
		// DEDO 4
		// ==========================================================

		// Rectangulo 1
		model = glm::translate(modelTemp2, glm::vec3(-0.75f, -0.36f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo41), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 2
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo42), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 3
		model = glm::translate(modelTemp, glm::vec3(0.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo43), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==========================================================
		// PULGAR
		// ==========================================================

		// Rectangulo 1
		model = glm::translate(modelTemp2, glm::vec3(-0.75f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(pulgar1), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;
		model = glm::translate(model, glm::vec3(-0.25f, -0.35f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.7f, 0.2f));

		color = glm::vec3(1.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Rectangulo 2
		model = glm::translate(modelTemp, glm::vec3(-0.25f, -0.7f, 0.0f));
		model = glm::rotate(model, glm::radians(pulgar2), glm::vec3(0.0f, 0.0f, 1.0f));

		modelTemp = model;

		model = glm::translate(model, glm::vec3(0.0f, -0.35f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.7f, 0.2f));

		color = glm::vec3(1.0f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//// Rectangulo 3
		//model = glm::translate(modelTemp, glm::vec3(0.0f, -0.7f, 0.0f));
		//model = glm::rotate(model, glm::radians(pulgar3), glm::vec3(0.0f, 0.0f, 1.0f));

		//model = glm::translate(model, glm::vec3(0.0f, -0.35f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.7f, 0.2f));

		//color = glm::vec3(1.0f, 0.8f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// END
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		 hombro += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		 hombro -= 0.18f; if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	 {
		 if (hombro < 90.0f)
			 hombro += 0.18f;
	 }

	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	 {
		 if (hombro > -90.0f)
			 hombro -= 0.18f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	 {
		 if (codo < 130.0f)
			 codo += 0.18f;
	 }

	 if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	 {
		 if (codo > 0.0f)
			 codo -= 0.18f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	 {
		 if (muneca < 45.0f)
			 muneca += 0.18f;
	 }

	 if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	 {
		 if (muneca > -45.0f)
			 muneca -= 0.18f;
	 }
	 // SEGUNDA FALANGE DE LOS 4 DEDOS
	 if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	 {
		 if (dedo11 > -70.0f)
		 {
			 dedo11 -= 0.18f;
			 dedo21 -= 0.18f;
			 dedo31 -= 0.18f;
			 dedo41 -= 0.18f;
		 }
	 }

	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	 {
		 dedo12 += 0.18f;
		 dedo22 += 0.18f;
		 dedo32 += 0.18f;
		 dedo42 += 0.18f;
	 }


	 // TERCERA FALANGE DE LOS 4 DEDOS
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	 {
		 if (dedo11 < 0.0f)
		 {
			 dedo11 += 0.18f;
			 dedo21 += 0.18f;
			 dedo31 += 0.18f;
			 dedo41 += 0.18f;
		 }
	 }

	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	 {
		 if (dedo12 > -90.0f)
		 {
			 dedo12 -= 0.18f;
			 dedo22 -= 0.18f;
			 dedo32 -= 0.18f;
			 dedo42 -= 0.18f;
		 }
	 }

	 if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	 {
		 if (dedo11 < 0.0f)
		 {
			 dedo13 -= 0.18f;
			 dedo23 -= 0.18f;
			 dedo33 -= 0.18f;
			 dedo43 -= 0.18f;
		 }
	 }

	 if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	 {
		 if (dedo12 > -90.0f)
		 {
			 dedo13 -= 0.18f;
			 dedo23 -= 0.18f;
			 dedo33 -= 0.18f;
			 dedo43 -= 0.18f;
		 }
	 }
 }


