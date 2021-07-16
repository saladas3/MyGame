#pragma once

#include <Windows.h>
#include <exception>

class Window
{

public:
	// Create and release the window
	Window();
	~Window();

private:
	// Peeks and dispatches messages to the WndProc method created
	bool broadcast();

public:
	// Checks if the window is running
	bool isRunning();

	// Utility methods
	RECT getClientWindowRect();
	RECT getSizeScreen();

	// Window Events
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onResize();

protected:
	HWND mHwnd = nullptr;
	bool mIsRunning = false;
	bool mIsInit = false;

};

