#include "Input.h"

#include <imgui.h>
#include <unordered_map>
#include <utility>

Input::Input()
{
	memset(m_keyPressed, static_cast<int>(false), sizeof(bool) * MaxKeyCode);
}

bool Input::ContainsModifier(engine::KeyMod mod) const
{
	if (m_keyModifiers == engine::KeyMod::KMOD_NULL) {
		return false;
	}
	return static_cast<std::underlying_type_t<engine::KeyMod>>(m_keyModifiers) & static_cast<std::underlying_type_t<engine::KeyMod>>(mod);
}

void Input::SetModifier(engine::KeyMod mod)
{
	if (!ContainsModifier(mod))
	{
		m_keyModifiers = static_cast<engine::KeyMod>(static_cast<std::underlying_type_t<engine::KeyMod>>(m_keyModifiers) | static_cast<std::underlying_type_t<engine::KeyMod>>(mod));
	}
}

void Input::ClearModifier(engine::KeyMod mod)
{
	if (ContainsModifier(mod))
	{
		m_keyModifiers = static_cast<engine::KeyMod>(static_cast<std::underlying_type_t<engine::KeyMod>>(m_keyModifiers) & ~static_cast<std::underlying_type_t<engine::KeyMod>>(mod));
	}
}

void Input::SetKeyPressed(engine::KeyCode code, bool pressed)
{ 
	m_keyPressed[static_cast<std::underlying_type_t<engine::KeyCode>>(code)] = pressed;
}

void Input::AppendKeyEvent(engine::KeyCode code, engine::KeyMod mod, bool pressed)
{
	KeyEvent newEvent;
	newEvent.code = code;
	newEvent.mod = mod;
	newEvent.isPressed = pressed;
	m_keyEventList.push_back(newEvent);
}

void Input::AppendInputCharacter(const char* c, size_t len)
{
	if ((m_inputCharBufferIndex + len) >= MaxInputCharBuffer)
	{
		// overflown for this frame, ignore this input
		return;
	}
	for (size_t i = 0; i < len; ++i)
	{
		m_inputCharBuffer[m_inputCharBufferIndex + i] = c[i];
	}
	m_inputCharBuffer[m_inputCharBufferIndex + len] = '\0';
	m_inputCharBufferIndex += len;
}

void Input::FlushInputs()
{ 
	m_keyEventList.clear(); 
	m_inputCharBufferIndex = 0;
	m_inputCharBuffer[0] = '\0';
}