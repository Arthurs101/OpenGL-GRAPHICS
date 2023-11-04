

#include"Elements/Mesh.h"
#include "Elements/ObjModel.h"


const unsigned int width = 800;
const unsigned int height = 800;



// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


glm::vec4 updateColor(float time, glm::vec4 lightColor) {
	float red = sin(time) * 0.5f + 0.5f;     // Red component transitions from 1 to 0
	float green = sin(time + 2.0944f) * 0.5f + 0.5f;  // Green component transitions from 0 to 1
	float blue = sin(time + 4.1888f) * 0.5f + 0.5f;   // Blue component transitions from 0 to 1
	return glm::vec4(red, green, blue, 1.0f);
}


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);




	

	// Generates Shader object using shaders default.vert and default.frag
	//shaders for the object
	Shader defaultProgram("Resources\\Shaders\\default\\default.vert", "Resources\\Shaders\\default\\default.frag");
	Shader twisterProgram("Resources\\Shaders\\twister\\twist.vert", "Resources\\Shaders\\default\\default.frag");
	Shader waveProgram("Resources\\Shaders\\default\\default.vert", "Resources\\Shaders\\wavecolors\\wave.frag");
	Shader toonProgram("Resources\\Shaders\\default\\default.vert", "Resources\\Shaders\\toonshader\\toon.frag");
	Shader waveMProgram("Resources\\Shaders\\wavecolors\\turning.vert", "Resources\\Shaders\\wavecolors\\wameMult.frag");

	defaultProgram.Activate();
	twisterProgram.Activate();
	waveProgram.Activate();
	toonProgram.Activate();
	waveMProgram.Activate();

	Shader currShader = defaultProgram;
	Shader shaderFloor("Resources\\Shaders\\default\\default.vert", "Resources\\Shaders\\default\\default.frag");
	/*Shader shaderhiku("Resources\\Shaders\\default\\default.vert", "Resources\\Shaders\\default\\default.frag");*/
	// Store mesh data in vectors for the mesh
	std::string texPath = "Resources\\Textures\\";
	Texture rt[]
	{
		Texture((texPath + "BOTr.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texPath + "BOT.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texPath + "BOTd.png").c_str(), "diffuse", 2, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texPath + "planksSpec.png").c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE),	
	};
	// Texture data
	Texture textures[]
	{
		Texture((texPath + "planks.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texPath + "planksSpec.png").c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> tex2(rt, rt + sizeof(rt) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);


	// Shader for light cube
	Shader lightShader("Resources\\Shaders\\light\\light.vert", "Resources\\Shaders\\light\\light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	//obj
	// textures
	/*std::vector<Texture> hiko_text = {
		Texture((texPath + "planks.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texPath + "BOT.png").c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	};*/
	// Create light mesh
	Mesh light(lightVerts, lightInd, tex);
	

	// Load obj
	ObjModel cube(
		"Resources\\Objects\\robot.obj",
		glm::vec3(0.0f, 0.0f, 0.0f), // position
		glm::vec3(0.1f, 0.1f, 0.1f),   // scale
		glm::vec3(0.0f, 0.0f, 0.0f), // rotation (identity quaternion)
		tex2
	);
	std::vector<Texture> hiku = {
		Texture((texPath + "tank.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texPath + "planksSpec.png").c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	};
	//ObjModel hiko(
	//	"Resources\\Objects\\tank.obj",
	//	glm::vec3(0.5f, 0.0f, 0.0f), // position
	//	glm::vec3(0.1f, 0.1f, 0.1f),   // scale
	//	glm::vec3(0.0f, 0.0f, 0.0f), // rotation (identity quaternion)
	//	hiku
	//);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 2.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	currShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(currShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModelMatrix()));
	glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(currShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	shaderFloor.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderFloor.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderFloor.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderFloor.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	/*shaderhiku.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderhiku.ID, "model"), 1, GL_FALSE, glm::value_ptr(hiko.getModelMatrix()));
	glUniform4f(glGetUniformLocation(shaderhiku.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderhiku.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);*/
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		float elapsedTime = fmod(time, 6.0f);  // 6 seconds for a full cycle (2 seconds for each color)

		// Input handling
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			currShader = defaultProgram; // Switch to basic shader when 1 is pressed
			currShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(currShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModelMatrix()));
			glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(currShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			currShader = twisterProgram;
			currShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(currShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModelMatrix()));
			glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(currShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			currShader = waveProgram;
			currShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(currShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModelMatrix()));
			glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(currShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
			currShader = toonProgram;
			currShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(currShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModelMatrix()));
			glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(currShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
			currShader = waveMProgram;
			currShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(currShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModelMatrix()));
			glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(currShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		}
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//lightColor = updateColor(elapsedTime, lightColor);
		//update color
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform4f(glGetUniformLocation(shaderFloor.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform4f(glGetUniformLocation(currShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform1f(glGetUniformLocation(currShader.ID, "time"), time); //update time

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		// Draws different meshes
		floor.Draw(shaderFloor, camera);
		light.Draw(lightShader, camera);
		cube.Draw(currShader, camera);
		//hiko.Draw(shaderhiku, camera);
		


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	currShader.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
