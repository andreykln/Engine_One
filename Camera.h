#pragma once
#include "Window.h"


extern const short resolution_width;
extern const short resolution_height;

//default values
const float YAW = 45.5f;
const float PITCH = 0.0f;
const float SPEED = 20.5f;
const float SENSITIVITY = 0.009f;
const float FOV_max = DirectX::XM_PI * 0.25;
const float FOV_min = DirectX::XM_PI / 180.0f;
const DirectX::XMVECTOR defaultFront{ 0.0f, 0.0f, 5.0f };
const DirectX::XMVECTOR defaultPositon{ 0.0f, 0.0f, -6.0f };
const DirectX::XMVECTOR defaultUp{ 0.0f, 1.0f, 0.0f };


class Camera
{
public:
	 Camera(DirectX::XMVECTOR _pos = DirectX::XMVECTOR{ 0.0f, 0.0f, -6.0f }, DirectX::XMVECTOR _up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f },
	 	float _yaw = YAW, float _pitch = PITCH)
	 	: front(DirectX::XMVECTOR{ 0.0f, 0.0f, 5.0f }), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV_max)
{
		positon = _pos;
		worldUp = _up;
		yaw = _yaw;
		pitch = _pitch;
		UpdateCameraVectors();
	}

	 DirectX::XMMATRIX GetViewProjection(int xMouse, int yMouse, bool isLeftPressed, float deltaTime, Window& wnd)
	 {
		 ProcessMouseMovement(xMouse, yMouse, isLeftPressed);
		 ProcessKeyboard(deltaTime);
		 ProcessMouseScroll(wnd);
		 viewMatrix = GetViewMatrix();
		 perspectiveProjection = DirectX::XMMatrixPerspectiveFovLH(fov, (float)resolution_width / (float)resolution_height, 1.0f, 1000.0f);
		 return viewMatrix * perspectiveProjection;
	 }


private:
	DirectX::XMMATRIX GetViewMatrix()
	{
		using namespace DirectX;
		return DirectX::XMMatrixLookAtLH(positon, positon + front, up);
	}

	void ProcessMouseMovement(int xPos, int yPos, bool isLeftPressed)
	{
		if (firstMouse)
		{
			lastX = (float)xPos;
			lastY = (float)yPos;
			firstMouse = false;
		}
		float xOffset = (float)xPos - (float)lastX;
		float yOffset = (float)yPos - (float)lastY;
		lastX = (float)xPos;
		lastY = (float)yPos;

		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		//reset to original position
		if (isLeftPressed)
		{
			yaw = YAW;
			pitch = PITCH;
			front = defaultFront;
			positon = defaultPositon;
			up = defaultUp;
			fov = FOV_max;
		}
		UpdateCameraVectors();
	}

	void ProcessKeyboard(float deltaTime)
	{
		using namespace DirectX;
		const float velocity = movementSpeed * deltaTime;
		if (GetAsyncKeyState('W') & 0x8000)
		{
			positon += front * velocity;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			positon -= velocity * front;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			positon -= right * velocity;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			positon += right * velocity;
		}
	}

	void ProcessMouseScroll(Window& wnd)
	{
		while (!wnd.mouse.IsEmpty())
		{
			const Mouse::Event e = wnd.mouse.Read();

			switch (e.GetType())
			{
			case Mouse::Event::Type::MWheelUp:
			{
				fov += 0.1f;
				if (fov > FOV_max)
				{
					fov = FOV_max;
				}
				break;
			}
			

			case Mouse::Event::Type::MWheelDown:
			{
				fov -= 0.1f;
				if (fov < FOV_min)
				{
					fov = FOV_min;
				}
				break;
			}
			}
		}

		/*fov = zoom;
		if (fov < DirectX::XM_PI / 180.0f)
		{
			fov = DirectX::XM_PI / 180.0f;
		}
		if (fov > FOV)
		{
			fov = FOV;
		}*/
	}

	void UpdateCameraVectors()
	{
		//calculate the new front vector
		front = DirectX::XMVECTOR{ -cos(yaw) * cos(pitch), -sin(pitch), sin(yaw) * cos(pitch) };
		front = DirectX::XMVector3Normalize(front);
		//re-calculate up and right
		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(front, worldUp));
		up = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(right, front));
	}
	

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX perspectiveProjection;
	DirectX::XMVECTOR positon;
	DirectX::XMVECTOR front;
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR worldUp;
	//Euler angles
	float yaw;
	float pitch;
	//camera options
	float movementSpeed = 0.0f;
	float mouseSensitivity = 0.0f;
	float fov = FOV_max;
	//mouse
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;



};
