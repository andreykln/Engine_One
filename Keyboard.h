#pragma once
#include <queue>
#include <bitset>

class Window;


class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Release,
			Press,
			Invalid
		};
		Event() : code(0u), type(Type::Invalid) {}
		Event(unsigned char in_code, Keyboard::Event::Type in_type)
			: type(in_type), code(in_code)  {}

		bool IsPressed() noexcept
		{
			return Event::Type::Press == type;
		}
		bool IsReleased() noexcept
		{
			return Event::Type::Release == type;
		}
		bool IsValid() noexcept
		{
			return !(Event::Type::Invalid == type);
		}
	private:
		unsigned char code;
		Type type;
	};

	Keyboard() = default;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard(const Keyboard&) = delete;

	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnCharPressed(unsigned char keycode) noexcept;
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	bool KeyQueueIsEmpty() const noexcept;
	Keyboard::Event ReadKey() noexcept;
	void ClearKeyQueue() noexcept;
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	bool CharQueueIsEmpty() const noexcept;
	void ClearCharQueue() noexcept;
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeate() noexcept;
	bool IsAutoRepeatEnabled() const noexcept;
	template<typename T> void TrimBuffer(std::queue<T>& queue) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int k_Buffersize = 16u;
	bool k_AutoRepeatEnabled = false;
	std::bitset<nKeys> k_KeyStates;
	std::queue<Event> k_keyEventBuffer;
	std::queue<char> k_Charbuffer;
};
