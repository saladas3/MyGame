#include "InputSystem.h"

// Input System is a singleton
InputSystem* InputSystem::mInputSystem = nullptr;

InputSystem::InputSystem() = default;

InputSystem::~InputSystem()
{
    InputSystem::mInputSystem = nullptr;
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	// Check if the mouse is moved for the first time
	if (mFirstTime)
	{
		mOldMousePos = Point(current_mouse_pos.x, current_mouse_pos.y);
		mFirstTime = false;
	}

	// There is a mouse move event
	if (current_mouse_pos.x != mOldMousePos.m_x || current_mouse_pos.y != mOldMousePos.m_y)
	{
		auto it = mSetListeners.begin();

		while (it != mSetListeners.end())
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}

	mOldMousePos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (::GetKeyboardState(mKeysState))
	{
		for (int i = 0; i < 256; i++)
		{
			if (mKeysState[i] & 0x80) // KEY IS DOWN
			{
				auto it = mSetListeners.begin();

				while (it != mSetListeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (mKeysState[i] != mOldKeysState[i])
							(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						if (mKeysState[i] != mOldKeysState[i])
							(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
						(*it)->onKeyDown(i);

					++it;
				}
			}
			else // KEY IS UP
			{
				if (mKeysState[i] != mOldKeysState[i])
				{
					auto it = mSetListeners.begin();

					while (it != mSetListeners.end())
					{
						if (i == VK_LBUTTON)
						{
							if (mKeysState[i] != mOldKeysState[i])
								(*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == VK_RBUTTON)
						{
							if (mKeysState[i] != mOldKeysState[i])
								(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else
							(*it)->onKeyUp(i);
						++it;
					}
				}
			}

		}
		// Store current keys state to old keys state buffer
		::memcpy(mOldKeysState, mKeysState, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	this->mSetListeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	this->mSetListeners.erase(listener);
}

void InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
    return mInputSystem;
}

void InputSystem::create()
{
	if (InputSystem::mInputSystem)
		throw std::exception("Input System already created");
	InputSystem::mInputSystem = new InputSystem();
}

void InputSystem::release()
{
	if (!InputSystem::mInputSystem) return;
	delete InputSystem::mInputSystem;
}
