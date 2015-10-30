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

	Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), wheelSensitivity(WHEELSENSITIVTY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	GLfloat Camera::GetZoom()
	{
		return this->zoom;
	}

	void Camera::Move(CameraMovementDirection direction, GLfloat deltaTime)
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

	void Camera::Rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch)
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

	void Camera::Zoom(GLfloat yoffset)
	{
		yoffset *= this->wheelSensitivity;
		if (this->zoom >= MINFOV && this->zoom <= MAXFOV)
			this->zoom -= yoffset;
		if (this->zoom <= MINFOV)
			this->zoom = MINFOV;
		if (this->zoom >= MAXFOV)
			this->zoom = MAXFOV;
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
}