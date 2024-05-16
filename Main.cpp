//------- Ignore this ----------
#include<filesystem>
#include<vector>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Solar System", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(700.0f, 0.0f, 545.0f));

	// Loads all planet models
	std::string resourcePath = (fs::current_path()).string() + "\\Resources\\";
	std::vector<Model> planets;
	std::vector<std::string> planetNames = {"mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune"};

	planets.push_back(Model((resourcePath + "skybox\\skybox.gltf").c_str(), glm::vec3(0.f, 0.f, 0.f), 1000.f));
	planets.push_back(Model((resourcePath + "sun\\sun.gltf").c_str(), glm::vec3(0.f, 0.f, 0.f), 10.f));

	for (auto& planetName : planetNames) {
		std::string modelPath = planetName + "\\" + planetName + ".gltf";
		planets.push_back(Model((resourcePath + modelPath).c_str(), glm::vec3(0.f, 0.f, 0.f), 50.f));
	}
	
	// Sets different values for every planet/model
	planets[0].isLightSource = true;
	planets[1].isLightSource = true;

	float sunRadius = 545.f;
	float baseRadius = 1000.f;
	
	planets[2].orbitRadius = sunRadius + baseRadius * 0.39f / 2.f;
	planets[3].orbitRadius = sunRadius + baseRadius * 0.72f / 2.f;
	planets[4].orbitRadius = sunRadius + baseRadius;
	planets[5].orbitRadius = sunRadius + baseRadius * 1.52f / 2.f;
	planets[6].orbitRadius = sunRadius + baseRadius * 5.2f / 2.f;
	planets[7].orbitRadius = sunRadius + baseRadius * 9.54f / 2.f;
	planets[8].orbitRadius = sunRadius + baseRadius * 19.2f / 2.f;
	planets[9].orbitRadius = sunRadius + baseRadius * 30.06f / 2.f;

	float baseSpeed = 1.f;

	planets[2].orbitSpeed = baseSpeed * 1.59f;
	planets[3].orbitSpeed = baseSpeed * 1.18f;
	planets[4].orbitSpeed = baseSpeed;
	planets[5].orbitSpeed = baseSpeed * 0.808f;
	planets[6].orbitSpeed = baseSpeed * 0.439f;
	planets[7].orbitSpeed = baseSpeed * 0.325;
	planets[8].orbitSpeed = baseSpeed * 0.228;
	planets[9].orbitSpeed = baseSpeed * 0.182;

	planets[1].rotationSpeed = baseSpeed * 24.47f;
	planets[2].rotationSpeed = baseSpeed * 58.8f;
	planets[3].rotationSpeed = baseSpeed * -244.f;
	planets[4].rotationSpeed = baseSpeed;
	planets[5].rotationSpeed = baseSpeed * 1.03f;
	planets[6].rotationSpeed = baseSpeed * 0.415f;
	planets[7].rotationSpeed = baseSpeed * 0.445f;
	planets[8].rotationSpeed = baseSpeed * -0.72f;
	planets[9].rotationSpeed = baseSpeed * 0.673f;
	
	// Is simulation paused?
	bool paused = false;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 1000000.0f);

		// Draws models
		for (auto& planet : planets) {
			if (!paused)
				planet.Update(glfwGetTime());
			planet.Draw(shaderProgram, camera);
		}
			
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}