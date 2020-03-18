#include "Keyboard.h"

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	k_KeyStates[keycode] = true;
	k_keyEventBuffer.push(Keyboard::Event(keycode, Keyboard::Event::Type::Press));
	TrimBuffer(k_keyEventBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	k_KeyStates[keycode] = false;
	k_keyEventBuffer.push(Keyboard::Event(keycode, Keyboard::Event::Type::Release));
	TrimBuffer(k_keyEventBuffer);
}

void Keyboard::OnCharPressed(unsigned char keycode) noexcept
{
	k_Charbuffer.push(keycode);
	TrimBuffer(k_Charbuffer);
}



bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return k_KeyStates[keycode];
}

bool Keyboard::KeyQueueIsEmpty() const noexcept
{
	return k_keyEventBuffer.empty();
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (!k_keyEventBuffer.empty())
	{
		Keyboard::Event e = k_keyEventBuffer.front(); 
		k_keyEventBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

void Keyboard::ClearKeyQueue() noexcept
{
	k_keyEventBuffer = std::queue<Event>();
}

char Keyboard::ReadChar()  noexcept
{
	if (!k_Charbuffer.empty())
	{
		unsigned char charcode = k_Charbuffer.front();
		k_Charbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return k_Charbuffer.empty();
}

bool Keyboard::CharQueueIsEmpty() const noexcept
{
	return k_Charbuffer.empty();
}

void Keyboard::ClearCharQueue() noexcept
{
	k_Charbuffer = std::queue<char>();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	k_AutoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeate() noexcept
{
	k_AutoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled() const noexcept
{
	return k_AutoRepeatEnabled;
}

void Keyboard::ClearState() noexcept
{
	k_KeyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& queue) noexcept
{
	while (queue.size() > k_Buffersize)
	{
		queue.pop();
	}
}
