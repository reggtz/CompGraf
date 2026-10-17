
// Gutierrez Benavides Frida Regina
// 320233454
// Cinco fuentes: un sol direccional, tres puntuales y un reflector conico.
// Copia independiente del escenario original

// Std. Includes
#include <string>
#include <iostream>
#include <cstdlib>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
// Vista inicial mas amplia para incluir los cinco elementos.
Camera camera(glm::vec3(-4.0f, 4.2f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -6.0f);
bool keys[1024] = {};
bool solEncendido = true;
bool posteIzquierdoEncendido = true;
bool posteDerechoEncendido = true;
bool cofreEncendido = true;
bool faroEncendido = true;
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::mat4 MatrizModelo(
    glm::vec3 posicion, glm::vec3 rotacion, glm::vec3 escala,
    glm::vec3 origen = glm::vec3(0.0f))
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, posicion);

    model = glm::rotate(
        model, glm::radians(rotacion.z),
        glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::rotate(
        model, glm::radians(rotacion.y),
        glm::vec3(0.0f, 1.0f, 0.0f));

    model = glm::rotate(
        model, glm::radians(rotacion.x),
        glm::vec3(1.0f, 0.0f, 0.0f));

    model = glm::scale(model, escala);
    // Centrar el modelo y colocar su base en Y=0 antes de escalarlo.
    model = glm::translate(model, -origen);
    return model;
}

void DibujarModelo(
    Model& objeto, Shader& shader,
    glm::vec3 posicion, glm::vec3 rotacion, glm::vec3 escala,
    glm::vec3 origen = glm::vec3(0.0f))
{
    glm::mat4 model = MatrizModelo(posicion, rotacion, escala, origen);
    glUniformMatrix4fv(
        glGetUniformLocation(shader.Program, "model"),
        1, GL_FALSE, glm::value_ptr(model));

    objeto.Draw(shader);
}

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Carga de modelos y camara sintetica Frida Regina Gutierrez Benavides", nullptr, nullptr);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/poste_lighting.vs", "Shader/poste_lighting.frag");
    Shader solShader("Shader/poste_lamp.vs", "Shader/poste_lamp.frag");

    GLint enlazado = GL_FALSE;
    glGetProgramiv(shader.Program, GL_LINK_STATUS, &enlazado);
    if (enlazado != GL_TRUE)
    {
        std::cerr << "Revisa los errores de Shader/poste_lighting.frag." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glGetProgramiv(solShader.Program, GL_LINK_STATUS, &enlazado);
    if (enlazado != GL_TRUE)
    {
        std::cerr << "Revisa los shaders del modelo del sol." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Cargar una sola vez, antes del while.
    Model calle((char*)"Models/Calle/low poly roads.obj");
    Model montana((char*)"Models/Montaña/Mountain.obj");
    Model patineta((char*)"Models/Patineta/skateboard_by_dommk.obj");
    Model perro((char*)"Models/Perro/12228_Dog_v1_L2.obj");
    Model cofre((char*)"Models/Cofre/treasure_chest_obj.obj");
    Model autoModelo((char*)"Models/Auto/dodge_HP.obj");

    // VALORES PARA MODIFICAR
    // Posicion: X = izquierda/derecha
    //           Y = abajo/arriba
    //           Z = fondo/frente

    glm::vec3 posicionCalle(0.0f, -0.12f, 0.0f);
    glm::vec3 rotacionCalle(0.0f, 90.0f, 0.0f);
    // Ampliar la superficie para apoyar el auto completo y cubrir el nuevo encuadre.
    glm::vec3 escalaCalle(3.0f, 1.0f, 2.0f);

    glm::vec3 posicionMontana(0.0f, 0.0f, -20.0f);
    glm::vec3 rotacionMontana(0.0f);
    glm::vec3 escalaMontana(1.5f);

    // La patineta viene alargada sobre Z.
    // Giramos para que quede atravesada sobre X.
    glm::vec3 posicionPatineta(0.0f, 0.32f, 0.0f);
    glm::vec3 rotacionPatineta(0.0f, 90.0f, 0.0f);
    glm::vec3 escalaPatineta(0.005f);

    // Este perro viene con la altura sobre Z.
    // Lo giramos para que quede de pie sobre Y.
    glm::vec3 posicionPerro(0.0f, 0.35f, 0.0f);
    glm::vec3 rotacionPerro(-90.0f, -90.0f, 0.0f);
    glm::vec3 escalaPerro(0.055f);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.GetZoom()),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f,
        200.0f
    );

    Model cielo((char*)"Models/Cielo/cielo.obj");
    Model sol((char*)"Models/Sol/esfera.obj");
    // Copia de prueba sin mapas externos; el modelo original queda intacto.
    Model poste((char*)"Models/Lamp/Street_Lamp_prueba.obj");
    // Dos instancias del mismo modelo, a los lados del perro y mas al fondo.
    // Valores iniciales para la banqueta: ajustar Z y altura con una captura.
    glm::vec3 posicionPosteIzquierdo(-3.0f, 2.415555f, -3.5f);
    glm::vec3 posicionPosteDerecho(3.0f, 2.415555f, -3.5f);
    glm::vec3 escalaPoste(1.5f);
    // Y de la base = posicion.y - 1.61037 * escala.y = 0.0.
    // Si la banqueta tiene otra altura, sumar esa diferencia a posicion.y.
    glm::vec3 focoLocal(0.0f, 1.2738645f, 0.0f);
    glm::vec3 focoIzquierdo = posicionPosteIzquierdo + focoLocal * escalaPoste;
    glm::vec3 focoDerecho = posicionPosteDerecho + focoLocal * escalaPoste;

    // Esfera de radio 1. Posicion y escala del modelo visible.
    glm::vec3 posicionSol(3.0f, 5.0f, -8.0f);
    glm::vec3 escalaSol(0.65f);
    // Los rayos viajan desde el sol hacia el centro de la escena.
    glm::vec3 direccionSol = glm::normalize(glm::vec3(0.0f) - posicionSol);

    // Cuarta fuente: cofre sobre la ladera delantera de la montana.
    // Altura e inclinacion aproximadas de la ladera en X=-7, Z=-14.
    glm::vec3 posicionCofre(-7.0f, 5.75f, -14.0f);
    glm::vec3 rotacionCofre(18.7f, 0.0f, 34.4f);
    glm::vec3 escalaCofre(0.20f);
    glm::vec3 origenCofre(0.0049875f, 0.035405f, -0.0069915f);
    glm::mat4 matrizCofre = MatrizModelo(posicionCofre, rotacionCofre, escalaCofre, origenCofre);
    // Punto de luz en la parte frontal del cofre, separado de su superficie.
    glm::vec3 focoCofre = glm::vec3(matrizCofre * glm::vec4(0.0f, 3.0f, 4.9f, 1.0f));

    // Quinta fuente: un reflector que representa el haz frontal del auto.
    // Origen en el centro de la base; largo 10.05, ancho 4.71 y alto 3.50 unidades.
    glm::vec3 posicionAuto(-7.8f, 0.03f, 0.0f);
    // El frente local +Z gira hacia +X: el auto queda frente al hocico del perro.
    glm::vec3 rotacionAuto(0.0f, 90.0f, 0.0f);
    glm::vec3 escalaAuto(9.0f);
    glm::vec3 origenAuto(-0.000049f, -0.194373f, 0.000466f);
    glm::mat4 matrizAuto = MatrizModelo(posicionAuto, rotacionAuto, escalaAuto, origenAuto);
    glm::vec3 focoAuto = glm::vec3(matrizAuto * glm::vec4(0.0f, -0.035f, 0.56f, 1.0f));
    // W=0 transforma una direccion sin aplicarle la traslacion.
    glm::vec3 direccionAuto = glm::normalize(glm::vec3(matrizAuto * glm::vec4(0.0f, -0.45f, 1.0f, 0.0f)));

    std::cout << "Luces: L sol, P poste izquierdo, O poste derecho, C cofre, F faro." << std::endl;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // Fondo azul: aproxima el cielo.
        glClearColor(0.35f, 0.55f, 0.78f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glUniform1i(glGetUniformLocation(shader.Program, "sinIluminacion"), 0);
        glUniform3f(glGetUniformLocation(shader.Program, "emision"), 0.0f, 0.0f, 0.0f);
        glUniform1i(glGetUniformLocation(shader.Program, "usarColorPlano"), 0);
        glUniform1i(glGetUniformLocation(shader.Program, "esCristal"), 0);
        glUniform1i(glGetUniformLocation(shader.Program, "transparency"), 0);
        glUniform1f(glGetUniformLocation(shader.Program, "opacity"), 1.0f);
        glUniform1i(glGetUniformLocation(shader.Program, "texture_diffuse1"), 0);
        glm::vec3 observador = camera.GetPosition();
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"),
            observador.x, observador.y, observador.z);

        // Primera fuente: sol direccional. L permite comparar encendido/apagado.
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"),
            direccionSol.x, direccionSol.y, direccionSol.z);
        float intensidad = solEncendido ? 1.0f : 0.0f;
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"),
            0.20f * intensidad, 0.10f * intensidad, 0.04f * intensidad);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"),
            0.90f * intensidad, 0.40f * intensidad, 0.10f * intensidad);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"),
            0.30f * intensidad, 0.18f * intensidad, 0.08f * intensidad);

        // Inicializar las cuatro posiciones; se usan 0 y 1 para postes, 2 para el cofre.
        // constant = 1 evita dividir entre cero al calcular atenuacion.
        for (int i = 0; i < 4; ++i)
        {
            std::string nombre = "pointLights[" + std::to_string(i) + "].";
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "position").c_str()), 0.0f, 100.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "diffuse").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "specular").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (nombre + "constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (nombre + "linear").c_str()), 0.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (nombre + "quadratic").c_str()), 0.0f);
        }
        // Segunda y tercera fuentes: cada poste tiene su propia luz puntual.
        glm::vec3 posicionesFocos[2] = { focoIzquierdo, focoDerecho };
        bool encendidos[2] = { posteIzquierdoEncendido, posteDerechoEncendido };
        for (int i = 0; i < 2; ++i)
        {
            std::string nombre = "pointLights[" + std::to_string(i) + "].";
            float intensidadPoste = encendidos[i] ? 1.0f : 0.0f;
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "position").c_str()),
                posicionesFocos[i].x, posicionesFocos[i].y, posicionesFocos[i].z);
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "ambient").c_str()),
                0.025f * intensidadPoste, 0.04f * intensidadPoste, 0.07f * intensidadPoste);
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "diffuse").c_str()),
                0.35f * intensidadPoste, 0.65f * intensidadPoste, 1.0f * intensidadPoste);
            glUniform3f(glGetUniformLocation(shader.Program, (nombre + "specular").c_str()),
                0.35f * intensidadPoste, 0.65f * intensidadPoste, 1.0f * intensidadPoste);
            glUniform1f(glGetUniformLocation(shader.Program, (nombre + "constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (nombre + "linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(shader.Program, (nombre + "quadratic").c_str()), 0.032f);
        }

        // Luz puntual amarilla del cofre: ilumina tambien la montana.
        float intensidadCofre = cofreEncendido ? 1.0f : 0.0f;
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].position"), focoCofre.x, focoCofre.y, focoCofre.z);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].ambient"), 0.08f * intensidadCofre, 0.05f * intensidadCofre, 0.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].diffuse"), 2.5f * intensidadCofre, 1.8f * intensidadCofre, 0.08f * intensidadCofre);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].specular"), 1.0f * intensidadCofre, 0.8f * intensidadCofre, 0.1f * intensidadCofre);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].linear"), 0.14f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].quadratic"), 0.07f);

        // Reflector fijo al auto, independiente de la camara.
        float intensidadFaro = faroEncendido ? 1.0f : 0.0f;
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.position"), focoAuto.x, focoAuto.y, focoAuto.z);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.direction"), direccionAuto.x, direccionAuto.y, direccionAuto.z);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 2.4f * intensidadFaro, 2.3f * intensidadFaro, 1.9f * intensidadFaro);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 1.0f * intensidadFaro, 0.95f * intensidadFaro, 0.8f * intensidadFaro);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));

        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));

        DibujarModelo(
            montana, shader,
            posicionMontana, rotacionMontana, escalaMontana);

        DibujarModelo(
            calle, shader,
            posicionCalle, rotacionCalle, escalaCalle);

        DibujarModelo(
            patineta, shader,
            posicionPatineta, rotacionPatineta, escalaPatineta);

        DibujarModelo(
            perro, shader,
            posicionPerro, rotacionPerro, escalaPerro);


        // Auto con su textura original. Cofre dorado sin textura externa.
        DibujarModelo(autoModelo, shader, posicionAuto, rotacionAuto, escalaAuto, origenAuto);
        glUniform1i(glGetUniformLocation(shader.Program, "usarColorPlano"), 1);
        glUniform3f(glGetUniformLocation(shader.Program, "colorPlano"), 0.65f, 0.38f, 0.06f);
        glUniform3f(glGetUniformLocation(shader.Program, "emision"),
            0.28f * intensidadCofre, 0.18f * intensidadCofre, 0.01f * intensidadCofre);
        DibujarModelo(cofre, shader, posicionCofre, rotacionCofre, escalaCofre, origenCofre);
        glUniform3f(glGetUniformLocation(shader.Program, "emision"), 0.0f, 0.0f, 0.0f);

        // Poste sin textura: un color gris iluminado por las mismas fuentes.
        glUniform1i(glGetUniformLocation(shader.Program, "usarColorPlano"), 1);
        glUniform3f(glGetUniformLocation(shader.Program, "colorPlano"), 0.55f, 0.55f, 0.58f);
        DibujarModelo(poste, shader, posicionPosteIzquierdo, glm::vec3(0.0f), escalaPoste);
        DibujarModelo(poste, shader, posicionPosteDerecho, glm::vec3(0.0f), escalaPoste);
        glUniform1i(glGetUniformLocation(shader.Program, "usarColorPlano"), 0);

        // ===== DIBUJAR EL CIELO =====
        // El fondo conserva su textura sin recibir la luz del sol.
        glUniform1i(glGetUniformLocation(shader.Program, "sinIluminacion"), 1);
        glm::mat4 modeloCielo(1.0f);

        // Posicion: detras de la montaña.
        modeloCielo = glm::translate(
            modeloCielo,
            glm::vec3(0.0f, 5.0f, -45.0f)
        );

        // Tamaño del fondo. El plano ya es vertical.
        modeloCielo = glm::scale(
            modeloCielo,
            glm::vec3(40.0f, 25.0f, 1.0f)
        );

        // Enviar la matriz al shader.
        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modeloCielo)
        );

        // Dibujar.
        cielo.Draw(shader);
        // ===== FIN DEL CIELO =====

        // La esfera usa el shader de lamp: color constante, sin textura.
        solShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(solShader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(solShader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(solShader.Program, "colorLuz"),
            solEncendido ? 1.0f : 0.15f, solEncendido ? 0.45f : 0.07f, 0.08f);
        DibujarModelo(sol, solShader, posicionSol, glm::vec3(0.0f), escalaSol);

        // Indicadores visibles: el color constante no sustituye a la luz real.
        glUniform3f(glGetUniformLocation(solShader.Program, "colorLuz"),
            cofreEncendido ? 1.0f : 0.12f, cofreEncendido ? 0.85f : 0.08f, 0.02f);
        DibujarModelo(sol, solShader, focoCofre, glm::vec3(0.0f), glm::vec3(0.10f));
        glUniform3f(glGetUniformLocation(solShader.Program, "colorLuz"),
            faroEncendido ? 1.0f : 0.12f, faroEncendido ? 0.96f : 0.12f, faroEncendido ? 0.8f : 0.10f);
        // Dos focos visibles, un solo reflector: cinco fuentes en total.
        for (int lado = -1; lado <= 1; lado += 2)
        {
            glm::vec3 faroVisible = glm::vec3(matrizAuto * glm::vec4(0.17f * lado, -0.035f, 0.56f, 1.0f));
            DibujarModelo(sol, solShader, faroVisible, glm::vec3(0.0f), glm::vec3(0.09f));
        }

        glfwSwapBuffers(window);
    }

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


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        solEncendido = !solEncendido;

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        posteIzquierdoEncendido = !posteIzquierdoEncendido;

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        posteDerechoEncendido = !posteDerechoEncendido;

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        cofreEncendido = !cofreEncendido;

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        faroEncendido = !faroEncendido;

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

