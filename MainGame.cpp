#include "MainGame.h"

MainGame::MainGame() = default;

MainGame::~MainGame() = default;

void MainGame::onCreate()
{
	Window::onCreate();
	RECT rc = this->getClientWindowRect();
	mSwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->mHwnd, rc.right - rc.left, rc.bottom - rc.top);

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->mSwapChain, 0, 0.3f, 0.4f, 1);

}

void MainGame::onUpdate()
{
	Window::onUpdate();

	// Start swapping between the back and front buffer and present the rendered images on screen
	this->mSwapChain->present(false);
}

void MainGame::onDestroy()
{
	Window::onDestroy();
}

void MainGame::onFocus()
{
	Window::onFocus();
}

void MainGame::onKillFocus()
{
	Window::onKillFocus();
}

void MainGame::onResize()
{
	Window::onResize();
}

void MainGame::onKeyDown(int key)
{
}

void MainGame::onKeyUp(int key)
{
}

void MainGame::onMouseMove(const Point& mouse_pos)
{
}

void MainGame::onLeftMouseDown(const Point& mouse_pos)
{
}

void MainGame::onLeftMouseUp(const Point& mouse_pos)
{
}

void MainGame::onRightMouseDown(const Point& mouse_pos)
{
}

void MainGame::onRightMouseUp(const Point& mouse_pos)
{
}
