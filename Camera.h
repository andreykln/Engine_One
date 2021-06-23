#pragma once
#include "Window.h"


extern const short resolution_width;
extern const short resolution_height;

//default values
const float YAW = 45.5f;
const float PITCH = 0.0f;
const float SPEED = 20.5f;
const float SENSITIVITY = 0.009f;
const float FOV_max = DirectX::XM_PI * 0.33f;
const float FOV_min = DirectX::XM_PI / 180.0f;
const DirectX::XMVECTOR defaultFront{ 0.0f, 0.0f, 5.0f };
const DirectX::XMVECTOR defaultPositon{ 0.0f, 0.0f, -6.0f };
const DirectX::XMVECTOR defaultUp{ 0.0f, 1.0f, 0.0f };


class Camera
{
public:
	 Camera()
	 	: front(defaultFront), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV_max),
		 positon(defaultPositon), worldUp(defaultUp), yaw(YAW), pitch(PITCH)
	{
		UpdateCameraVectors();
	}

	 void SetLookAt(DirectX::XMFLOAT3* _pos, DirectX::XMFLOAT3* _up, DirectX::XMFLOAT3* _target)
	 {
		 positon = DirectX::XMLoadFloat3(_pos);
		 worldUp = DirectX::XMLoadFloat3(_up);
		 front = DirectX::XMLoadFloat3(_target);
		 yaw = YAW;
		 pitch = PITCH;
		 UpdateCameraVectors();
	 }
	 DirectX::XMFLOAT3 GetCameraPosition()
	 {
		 DirectX::XMFLOAT3 pos;
		 DirectX::XMStoreFloat3(&pos, positon);
		 return pos;
	 }

	 DirectX::XMFLOAT3 GetCameraDirection()
	 {
		 DirectX::XMFLOAT3 dir;
		 DirectX::XMStoreFloat3(&dir, front);
		 return dir;
	 }

	DirectX::XMMATRIX GetViewMatrix()
	{
		using namespace DirectX;
		return DirectX::XMMatrixLookAtLH(positon, positon + front, up);
	}
	DirectX::XMMATRIX GetProjecion() const
	{
		return perspectiveProjection;
	}

	DirectX::XMMATRIX GetViewProjectionWithMovement(int xMouse, int yMouse, float aspectRatio, float in_nearZ,
		bool isLeftPressed, float deltaTime, Window& wnd)
	{
		if (GetAsyncKeyState(0x01) & 0x8000)
		{
			ProcessMouseMovement(xMouse, yMouse, isLeftPressed);

		}
		ProcessKeyboard(deltaTime);
		ProcessMouseScroll(wnd);
		viewMatrix = GetViewMatrix();
		perspectiveProjection = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, in_nearZ, 1000.0f);
		return viewMatrix * perspectiveProjection;
	}

	DirectX::XMMATRIX SetAndGetViewProjection(float aspectRatio, float in_nearZ, Window& wnd)
	{
		viewMatrix = GetViewMatrix();
		perspectiveProjection = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, in_nearZ, 1000.0f);
		return viewMatrix * perspectiveProjection;
	}

private:
	void ProcessMouseMovement(int xPos, int yPos, bool isReset)
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
		if (GetAsyncKeyState('R'))
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
