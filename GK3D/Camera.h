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
		static const GLfloat YAW;
		static const GLfloat PITCH;
		static const GLfloat SPEED;
		static const GLfloat SENSITIVTY;
		static const GLfloat WHEELSENSITIVTY;
		static const GLfloat ZOOM;
		static const GLfloat MINFOV;
		static const GLfloat MAXFOV;

		Camera(int screenWidth, int screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
		Camera(int screenWidth, int screenHeight, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

		glm::mat4 getViewMatrix();
		GLfloat getZoom();
		void move(CameraMovementDirection direction, GLfloat deltaTime);
		void rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
		void zoom(GLfloat yoffset);
		int getScreenWidth();
		void setScreenWidth(int screenWidth);
		int getScreenHeight();
		void setScreenHeight(int screenHeight);
		glm::vec3 getPosition();
		void setPosition(glm::vec3 position);
		glm::vec3 getFront();
		void setFront(glm::vec3 front);

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
		GLfloat _zoom;
		int screenWidth;
		int screenHeight;

		void updateCameraVectors();
	};
}

#endif