#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);
	float angle = 0.0f; // Initialize the angle
	float rad = 0.0f;
	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;
	bool firstClickr = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);
	void calculateStartingAngle(glm::vec3 target);
	void ResetPos(glm::vec3 target, float Bradious, glm::vec3 minB, glm::vec3 maxB);
	// Updates the camera matrix to the Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	void LookAt(glm::vec3 target);
	void CircleMotion(float time, float FOVdeg, float nearPlane, float farPlane, glm::vec3 target);
	// Handles camera inputs
	void Inputs(GLFWwindow* window, glm::vec3 target , float boundingR , glm::vec3 minB, glm::vec3 maxB);
};
#endif