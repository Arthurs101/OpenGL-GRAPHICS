#include"../Camera.h"



Camera::Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	this->Projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
}

void Camera::calculateStartingAngle(glm::vec3 target)
{
	rad = glm::abs(glm::length(Position - target));
	angle = acos(Position.x / rad);
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::CircleMotion(float time, float FOVdeg, float nearPlane, float farPlane, glm::vec3 target) {
	float radius = 5.0f;  // Adjust the radius as needed
	float speed = 1.0f;   // Adjust the speed as needed

	// Parametric equations for a circle in 3D
	float x = radius * cos(speed * time);
	float z = radius * sin(speed * time);

	// Update the camera position
	this->Position = glm::vec3(x, Position.y, z);

	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, target, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	this->cameraMatrix = this->Projection * view;
}

void Camera::LookAt(glm::vec3 target) {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(this->Position, target, Up);
	this->cameraMatrix = Projection * view ;
}

glm::vec3 parametricPos(float r, float angle ,float y) {
	float x = r * cos(angle);
	float z = r * sin(angle);
	return glm::vec3(x, y, z);
}
void Camera::ResetPos(glm::vec3 target, float Bradious, glm::vec3 minB, glm::vec3 maxB) {
	this->rad = Bradious * 5; //place it on center point of zoom
	this->Position.y = (maxB.y + glm::abs(minB.y)) /2; //Place it on half height
	//set the new x,y based on the new zoom
	this->Position = parametricPos(this->rad, this->angle, Position.y);
}
void Camera::Inputs(GLFWwindow* window,glm::vec3 target, float Bradious , glm::vec3 minB, glm::vec3 maxB)
{
	// Handles key inputs
	if (Position.z < 0) {
		Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else {
		Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (rad - speed * 0.1 > Bradious) {
			rad -= 0.1;
			Position = parametricPos(rad,angle,Position.y);
		}

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		angle += speed;
		Position = parametricPos(rad, angle, Position.y);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{

		if (rad+speed*0.1 < glm::abs(Bradious*5)) {
			rad += speed ;
			Position = parametricPos(rad, angle, Position.y);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		angle -= 0.1 ;
		Position = parametricPos(rad, angle, Position.y);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if ((Position + speed * Up).y < maxB.y * 3) {
			Position += (0.1f*speed)* Up;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if ((Position + speed * -Up).y > minB.y  - minB.y * 2) {
			Position += (0.1f * speed) * -Up;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}
	

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}
		
		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseX - (width / 2)) / width;
		float rotY = sensitivity * (float)(mouseY - (height / 2)) / height;

		//move using the new rot values
		angle += rotX*0.01f;
		Position = parametricPos(rad, angle, Position.y);
		//move up and down based on the new offset
		if ((Position + (0.1f * speed) * rotY * Up).y < (maxB.y * 3)) {
			Position += (0.1f * speed) * rotY;
		}
		else if ((Position + (0.1f * speed) * rotY * Up).y > (minB.y - minB.y * 2)) {
			Position += (0.1f * speed) * rotY;
		}
		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{

		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClickr)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClickr = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotY = sensitivity * (float)(mouseY - (height / 2)) / height;
		//move up and down based on the new offset
		if (rad + speed * 0.1  * rotY > Bradious) {
			rad += speed * 0.1 * rotY;
			Position = parametricPos(rad, angle, Position.y);
		}
		else if (rad + speed * 0.1 * rotY < glm::abs(Bradious * 5)) {
			rad += speed * 0.1 * rotY;
			Position = parametricPos(rad, angle, Position.y);
		}
		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClickr = true;
	}
}