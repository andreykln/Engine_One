#pragma once
#include "Window.h"
//default values
const float YAW = 45.5f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.009f;
const float ZOOM = 45.0f;
const DirectX::XMVECTOR defaultFront{ 0.0f, 0.0f, 5.0f };
const DirectX::XMVECTOR defaultPositon{ 0.0f, 0.0f, -6.0f };
const DirectX::XMVECTOR defaultUp{ 0.0f, 1.0f, 0.0f };


class Window;

class Camera
{
public:
	 Camera(DirectX::XMVECTOR _pos = DirectX::XMVECTOR{ 0.0f, 0.0f, -6.0f }, DirectX::XMVECTOR _up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f },
	 	float _yaw = YAW, float _pitch = PITCH)
	 	: front(DirectX::XMVECTOR{ 0.0f, 0.0f, 5.0f }), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
		positon = _pos;
		worldUp = _up;
		yaw = _yaw;
		pitch = _pitch;
		UpdateCameraVectors();
	}

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
			yaw = 45.5f;
			pitch = 0.0f;
			front = defaultFront;
			positon = defaultPositon;
			up = defaultUp;
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

	void ProcessMouseScroll(float yOffset)
	{

	}

private:
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
	float zoom = 0.0f;
	//mouse
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;

};
