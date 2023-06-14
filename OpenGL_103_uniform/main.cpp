#define GLEW_STATIC 1

#include <iostream>

#if _MSC_VER
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT  
#endif

extern "C" {
	DLL_EXPORT uint32_t NvOptimusEnablement = 0x00000001;
	// DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

#include "../common/Mesh.h"
#include "../common/tiny_obj_loader.h"

// Vertices coordinates (deprecated)
//Vertex vertices[] =
//{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
//	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
//	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
//};

// Vertices coordinates (tiny_obj_loader)
std::vector<glm::vec3> vertices;

// Indices for vertices order (deprecated)
//GLuint indices[] =
//{
//	0, 1, 2,
//	0, 2, 3
//};

// Indices for vertices order (tiny_obj_loader)
std::vector<glm::vec3> indices;

const unsigned int width = 500;
const unsigned int height = 500;

int main() {
	// Initialize GLFW
	if(!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the core profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window of 500x500 pixels, with the name "Hello world"
	GLFWwindow* window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);

	// Error check if the window fails to create
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// We can now use GL commands

	// Lights related stuff
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	
	// Texture data
	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};

	// Generates Shader object using shaders default.vs.glsl and default.vs.glsl
	GLShader shaderProgram;
	shaderProgram.LoadVertexShader("default.vs.glsl");
	shaderProgram.LoadFragmentShader("default.fs.glsl");
	shaderProgram.Create();
	// Generates vertices and indices with tiny_obj_loader
	tinyobj::fillMatrices(vertices, indices);
	// Store mesh data in vectors for the mesh
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(vertices, indices, tex);
	glm::vec3 floorPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	glUseProgram(shaderProgram.GetProgram());
	glUniform4f(glGetUniformLocation(shaderProgram.GetProgram(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.GetProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 2.0f));
	// Loop until the user closes the window or another function request the window to close
	while (!glfwWindowShouldClose(window))
	{
		// Handle the current size of the window
		int m_width, m_height;
		glfwGetFramebufferSize(window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);

		
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draws different meshes
		floor.Draw(shaderProgram, camera, floorModel, floorPos);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Clear the app before ending the program
	shaderProgram.Destroy();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}