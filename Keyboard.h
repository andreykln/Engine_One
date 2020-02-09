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

		bool IsPressed()
		{
			return Event::Type::Press == type;
		}
		bool IsReleased()
		{
			return Event::Type::Release == type;
		}
		bool IsValid()
		{
			return !(Event::Type::Invalid == type);
		}
	private:
		unsigned char code;
		Type type;
	};







	void OnKeyPressed(unsigned char keycode);


	void OnKeyReleased(unsigned char keycode);
	void OnCharPressed(unsigned char keycode);
	bool KeyIsPressed(unsigned char keycode) const;
	bool KeyQueueIsEmpty() const;
	Keyboard::Event ReadKey();
	void ClearKeyQueue();
	char ReadChar();
	bool CharIsEmpty() const;
	bool CharQueueIsEmpty() const;
	void ClearCharQueue();
	void EnableAutoRepeat();
	void DisableAutoRepeate();
	bool IsAutoRepeatEnabled();
	template<typename T> void TrimBuffer(std::queue<T>& queue);
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int k_Buffersize = 16u;
	bool k_AutoRepeatEnabled = false;
	std::bitset<nKeys> k_KeyStates;
	std::queue<Event> k_keyEventBuffer;
	std::queue<char> k_Charbuffer;
};
