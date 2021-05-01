#include "Particle.h"
#include "ParticleSystem.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h""
#include <iostream>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

static ParticleSystem::ParticleSystemType SYSTEM_TYPE = ParticleSystem::ParticleSystemType::Fountain;
static int NUM_PARTICLES = 20;
static float SIZE_BALL = 0.2f;
static int solverInt = 2;
static Particle::UpdateMethod SOLVER = Particle::UpdateMethod::Verlet;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 17.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::vec3 lightPos(0.0f, 0.0f, 5.0f);
const char* glsl_version = "#version 130";

//std::cout << "Lifetime =" << p.getLifetime() << std::endl;
//p.setBouncing(1.0f);
//p.addForce(0, -9.8f, 0);
// One Plane

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("../resources/shaders/6.3.coordinate_systems.vs", "../resources/shaders/6.3.coordinate_systems.fs");

    Plane planes[6] = {
    Plane(0),
    Plane(1),
    Plane(2),
    Plane(5),
    Plane(4),
    Plane(3),
    };
    
    glm::vec3 pt1(3.5, 0, -3.5);
    glm::vec3 pt2(1.5, -2.5, -3.5);
    glm::vec3 pt3(4.5, -2.5, 1.5);
    Triangle triangle(pt1,pt2,pt3);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
    Sphere sphere;
    sphere.init(0, 2.0);
	sphere.setNewPos(0, -2.5, 0);


	ParticleSystem ps;
	ps.setParticleSystem(NUM_PARTICLES);
	ps.iniParticleSystem(SYSTEM_TYPE, SIZE_BALL);


	bool first = true;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        /////////////////////////////////////////////////////
        //////////////// UPDATE /////////////////////////////
        /////////////////////////////////////////////////////
		float currentFrame = glfwGetTime();

		if (first)
		{
			deltaTime = 0.02;
		}
		else
		{
			deltaTime = std::min(currentFrame - lastFrame, 0.2f);
		}
		first = false;

        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
		ps.updateParticleSystem(deltaTime, SOLVER);



		for (int indexPlane = 0; indexPlane < 6; ++indexPlane)
		{
			ps.checkColsPlane(planes[indexPlane]);
		}
		//ps.checkColsTriangle(triangle);
		//ps.checkColsSphere(sphere);
        ps.checkColsParticles();


        /////////////////////////////////////////////////////
        ////////////////  DRAW  /////////////////////////////
        /////////////////////////////////////////////////////
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // activate shader
        ourShader.use();

        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render models
        
        for (unsigned int i = 0; i < 6; i++)
        {
            planes[i].draw(&ourShader);
        }

		ps.drawParticles(&ourShader);
        ////////////////////////////////
        //// IMGUI /////////////////////
        ////////////////////////////////
        {
            //static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Options:");                          // Create a window called "Hello, world!" and append into it.

            
            ImGui::RadioButton("EulerOrig", &solverInt, 0); ImGui::SameLine();
            ImGui::RadioButton("EulerSemi", &solverInt, 1); ImGui::SameLine();
            ImGui::RadioButton("Verlet", &solverInt, 2);
            if (SIZE_BALL < 0.2)
            {
                ImGui::SliderInt("Num balls", &NUM_PARTICLES, 1, 250);            // Edit 1 float using a slider from 0.0f to 1.0f
            } 
            else if (SIZE_BALL < 0.4)
            {
                ImGui::SliderInt("Num balls", &NUM_PARTICLES, 1, 100);
                NUM_PARTICLES = std::min(100, NUM_PARTICLES);
            }
            else if (SIZE_BALL < 0.6)
            {
                ImGui::SliderInt("Num balls", &NUM_PARTICLES, 1, 50);
                NUM_PARTICLES = std::min(50, NUM_PARTICLES);
            }
            else if (SIZE_BALL < 0.8)
            {
                ImGui::SliderInt("Num balls", &NUM_PARTICLES, 1, 30);
                NUM_PARTICLES = std::min(30, NUM_PARTICLES);
            } 
            else
            {
                ImGui::SliderInt("Num balls", &NUM_PARTICLES, 1, 20);
                NUM_PARTICLES = std::min(20, NUM_PARTICLES);
            }
            ImGui::SliderFloat("Size ball", &SIZE_BALL, 0.1f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


            if (ImGui::Button("Restart Simulation"))
            {
                ps.setParticleSystem(NUM_PARTICLES);
                ps.iniParticleSystem(SYSTEM_TYPE, SIZE_BALL);
                SOLVER = static_cast<Particle::UpdateMethod>(solverInt);
                first = true;
                counter++;
            }
                

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}