#include "Keyboard.h"

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	k_KeyStates[keycode] = true;
	k_keyEventBuffer.push(Keyboard::Event(keycode, Keyboard::Event::Type::Press));
	TrimBuffer(k_keyEventBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	k_KeyStates[keycode] = false;
	k_keyEventBuffer.push(Keyboard::Event(keycode, Keyboard::Event::Type::Release));
	TrimBuffer(k_keyEventBuffer);
}

void Keyboard::OnCharPressed(unsigned char keycode)
{
	k_Charbuffer.push(keycode);
	TrimBuffer(k_Charbuffer);
}



bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return k_KeyStates[keycode];
}

bool Keyboard::KeyQueueIsEmpty() const
{
	return k_keyEventBuffer.empty();
}

Keyboard::Event Keyboard::ReadKey()
{
	if (!k_keyEventBuffer.empty())
	{
		Keyboard::Event e = k_keyEventBuffer.front(); // I was returning only this
		k_keyEventBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

void Keyboard::ClearKeyQueue()
{
	k_keyEventBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() 
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

bool Keyboard::CharIsEmpty() const
{
	return k_Charbuffer.empty();
}

bool Keyboard::CharQueueIsEmpty() const
{
	return k_Charbuffer.empty();
}

void Keyboard::ClearCharQueue()
{
	k_Charbuffer = std::queue<char>();
}

void Keyboard::EnableAutoRepeat()
{
	k_AutoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeate()
{
	k_AutoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled()
{
	return k_AutoRepeatEnabled;
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& queue)
{
	while (queue.size() > k_Buffersize)
	{
		queue.pop();
	}
}
