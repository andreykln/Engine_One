#pragma once
#include <queue>
#include "WindowsEdited.h"
#include "MathHelper.h"
//#include "Window.h"
class Window;
class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			MWheelUp,
			MWheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};

		Event() : type(Type::Invalid), x(0), y(0), leftIsPressed(false), rightIsPressed(false), middleIsPressed(false) {}
		Event(Type in_type, const Mouse& parent)
			: type(in_type), x(parent.x), y(parent.y),
			leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed), middleIsPressed(parent.middleIsPressed) {}
		Type GetType() const
		{
			return type;
		}
		bool IsValid() const
		{
			return type != Type::Invalid;
		}
		bool IsRightPressed() const
		{
			return type == Type::RPress;
		}
		bool IsLeftPressed() const 
		{
			return type == Type::LPress;
		}
		bool IsMiddlePressed() const
		{
			return type == Type::MPress;
		}
		int GetPosX() const
		{
			return x;
		}
		int GetPosY() const
		{
			return y;
		}
		std::pair<int, int> GetPosition() const
		{
			return { x,y };
		}
		

	private:
		Type type;
		int x;
		int y;
		bool leftIsPressed;
		bool rightIsPressed;
		bool middleIsPressed;

	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsMiddlePressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsInWindow() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Clear() noexcept;
private:
	void OnMouseMove(WPARAM btnState, int newx, int newy, float Theta, float Phi, float Radius) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnLeftPress(int x, int y) noexcept;
	void OnLeftRelease(int x, int y) noexcept;
	void OnRightPress(int x, int y) noexcept;
	void OnRightRelease(int x, int y) noexcept;
	void OnMiddlePress(int x, int y) noexcept;
	void OnMiddleRelease(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool middleIsPressed = false;
	bool inWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> eventBuffer;



};

