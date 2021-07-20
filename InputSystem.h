#pragma once

#include "InputListener.h"

#include <unordered_set>
#include <Windows.h>

class InputSystem
{

private:
	InputSystem();
	~InputSystem();

public:
    void update();
    void addListener(InputListener* listener);
    void removeListener(InputListener* listener);
    static void setCursorPosition(const Point& pos);
    static void showCursor(bool show);

public:
    static InputSystem* get();
    static void create();
    static void release();

private:
    std::unordered_set<InputListener*> mSetListeners;

    unsigned char mKeysState[256] = {};
    unsigned char mOldKeysState[256] = {};

    Point mOldMousePos;
    bool mFirstTime = true;

private:
    static InputSystem* mInputSystem;

};

