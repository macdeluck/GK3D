#include "Camera.h"

namespace GK
{
	const GLfloat Camera::MINFOV = 44.0f;
	const GLfloat Camera::MAXFOV = 45.0f;
	const GLfloat Camera::YAW = 90.0f;
	const GLfloat Camera::PITCH = 0.0f;
	const GLfloat Camera::SPEED = 0.001f;
	const GLfloat Camera::SENSITIVTY = 0.25f;
	const GLfloat Camera::WHEELSENSITIVTY = 0.1f;
	const GLfloat Camera::ZOOM = 45.0f;

	Camera::Camera(int screenWidth, int screenHeight, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
		: screenWidth(screenWidth), screenHeight(screenHeight), front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), wheelSensitivity(WHEELSENSITIVTY), _zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	Camera::Camera(int screenWidth, int screenHeight, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: screenWidth(screenWidth), screenHeight(screenHeight),
		front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), _zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	GLfloat Camera::getZoom()
	{
		return this->_zoom;
	}

	void Camera::move(CameraMovementDirection direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;
		if (direction == FORWARD)
			this->position += this->front * velocity;
		if (direction == BACKWARD)
			this->position -= this->front * velocity;
		if (direction == LEFT)
			this->position -= this->right * velocity;
		if (direction == RIGHT)
			this->position += this->right * velocity;
		if (direction == DOWN)
			this->position -= this->up * velocity;
		if (direction == UP)
			this->position += this->up * velocity;
	}

	void Camera::rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch)
	{
		xoffset *= this->mouseSensitivity;
		yoffset *= this->mouseSensitivity;

		this->yaw += xoffset;
		this->pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainpitch)
		{
			if (this->pitch > 89.0f)
				this->pitch = 89.0f;
			if (this->pitch < -89.0f)
				this->pitch = -89.0f;
		}

		// update front, right and up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	void Camera::zoom(GLfloat yoffset)
	{
		yoffset *= this->wheelSensitivity;
		if (this->_zoom >= MINFOV && this->_zoom <= MAXFOV)
			this->_zoom -= yoffset;
		if (this->_zoom <= MINFOV)
			this->_zoom = MINFOV;
		if (this->_zoom >= MAXFOV)
			this->_zoom = MAXFOV;
	}

	void Camera::updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

	int Camera::getScreenWidth()
	{
		return screenWidth;
	}
	void Camera::setScreenWidth(int screenWidth)
	{
		this->screenWidth = screenWidth;
	}
	int Camera::getScreenHeight()
	{
		return screenHeight;
	}
	void Camera::setScreenHeight(int screenHeight)
	{
		this->screenHeight = screenHeight;
	}

	glm::vec3 Camera::getPosition()
	{
		return position;
	}

	void Camera::setPosition(glm::vec3 position)
	{
		this->position = position;
	}

	glm::vec3 Camera::getFront()
	{
		return front;
	}

	void Camera::setFront(glm::vec3 front)
	{
		this->front = glm::normalize(front);
		this->pitch = glm::degrees(asin(front.y));
		this->yaw = glm::degrees(acos(front.x / cos(glm::radians(this->pitch))));
		updateCameraVectors();
	}
}