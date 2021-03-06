#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x,y };
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}



bool Mouse::IsLeftPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::IsMiddlePressed() const noexcept
{
	return middleIsPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return rightIsPressed;
}

bool Mouse::IsInWindow() const noexcept
{
	return inWindow;
}

Mouse::Event Mouse::Read() noexcept
{
	if (!eventBuffer.empty())
	{
		Mouse::Event e = eventBuffer.front();
		eventBuffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

bool Mouse::IsEmpty() const noexcept
{
	return eventBuffer.empty();
}

void Mouse::Clear() noexcept
{
	eventBuffer = std::queue<Event>();
}

void Mouse::OnMouseMove(WPARAM btnState, int newx, int newy, float& Theta, float& Phi, float& Radius) noexcept
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(newx - x));
		float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(newy - y));

		// Update angles based on input to orbit camera around box.
		Theta += dx;
		Phi += dy;

		// Restrict the angle mPhi.
		Phi = MathHelper::Clamp(Phi, 0.1f, DirectX::XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(newx - x);
		float dy = 0.005f * static_cast<float>(newy - y);

		// Update the camera radius based on input.
		Radius += dx - dy;

		// Restrict the radius.
		Radius = MathHelper::Clamp(Radius, 3.0f, 15.0f);
	}
	x = newx;
	y = newy;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}


void Mouse::OnMouseEnter() noexcept
{
	inWindow = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	inWindow = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnLeftPress(int x, int y) noexcept
{
	leftIsPressed = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftRelease(int x, int y) noexcept
{
	leftIsPressed = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPress(int x, int y) noexcept
{
	rightIsPressed = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightRelease(int x, int y) noexcept
{
	rightIsPressed = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePress(int x, int y) noexcept
{
	middleIsPressed = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddleRelease(int x, int y) noexcept
{
	middleIsPressed = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::MWheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::MWheelDown, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimBuffer() noexcept
{
	while (eventBuffer.size() > bufferSize)
	{
		eventBuffer.pop();
	}
}
