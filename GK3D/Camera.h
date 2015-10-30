#ifndef _GK_CAMERA
#define _GK_CAMERA

#include "Common.h"

namespace GK
{
	enum CameraMovementDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class Camera
	{
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
		Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

		glm::mat4 GetViewMatrix();
		GLfloat GetZoom();
		void Move(CameraMovementDirection direction, GLfloat deltaTime);
		void Rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
		void Zoom(GLfloat yoffset);

	private:
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		// Eular Angles
		GLfloat yaw;
		GLfloat pitch;
		// Camera options
		GLfloat movementSpeed;
		GLfloat mouseSensitivity;
		GLfloat wheelSensitivity;
		GLfloat zoom;
		static const GLfloat YAW;
		static const GLfloat PITCH;
		static const GLfloat SPEED;
		static const GLfloat SENSITIVTY;
		static const GLfloat WHEELSENSITIVTY;
		static const GLfloat ZOOM;
		static const GLfloat MINFOV;
		static const GLfloat MAXFOV;

		void updateCameraVectors();
	};
}

#endif