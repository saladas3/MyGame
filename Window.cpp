#include "Window.h"

// Main message handler for the program
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {

		case WM_CREATE:
		{
			// Event fired when the window is created
			break;
		}
		case WM_SIZE:
		{
			// Event fired when the windows is resized
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window) window->onResize();
			break;
		}
		case WM_SETFOCUS:
		{
			// Event fired when the window get focus
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window) window->onFocus();
			break;
		}
		case WM_KILLFOCUS:
		{
			// Event fired when the window lost focus
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onKillFocus();
			break;
		}
		case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}

		// Handle any messages the switch statement didn't
		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);

	}

	return NULL;
}

Window::Window()
{
	// Setting up WNDCLASSEX object - holds information for the window class
	WNDCLASSEX wc;

	// Clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = nullptr;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc;

	if (!::RegisterClassEx(&wc)) {
		throw std::exception("Window not created successfully");
	}

	// Creation of the window
	mHwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		1024, 768, nullptr, nullptr, nullptr, nullptr);

	// If the creation fails throw exception
	if (!mHwnd) {
		throw std::exception("Window not created successfully");
	}

	// Show the window
	::ShowWindow(mHwnd, SW_SHOW);
	::UpdateWindow(mHwnd);

	// Indicate that the window is initialized and running
	mIsRunning = true;
}

Window::~Window() = default;

bool Window::broadcast()
{
	MSG msg;

	if (!this->mIsInit) {
		// Changes an attribute of the specified window. 
		// The function also sets a value at the specified offset in the extra window memory.
		SetWindowLongPtr(mHwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->mIsInit = true;
	}

	if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	this->onUpdate();

	Sleep(1);

	return true;
}

bool Window::isRunning()
{
	if (mIsRunning) broadcast();
	return mIsRunning;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->mHwnd, &rc);
	::ClientToScreen(this->mHwnd, (LPPOINT)&rc.left);
	::ClientToScreen(this->mHwnd, (LPPOINT)&rc.right);
	return rc;
}

RECT Window::getSizeScreen()
{
	RECT rc;
	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return rc;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	mIsRunning = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

void Window::onResize()
{
}
