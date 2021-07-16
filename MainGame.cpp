#include "MainGame.h"

MainGame::MainGame() = default;

MainGame::~MainGame() = default;

// Temp - used for testing; DELETE AFTER
struct vertex {
	Vec3 position;
};

void MainGame::onCreate()
{
	Window::onCreate();
	RECT rc = this->getClientWindowRect();
	mSwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->mHwnd, rc.right - rc.left, rc.bottom - rc.top);

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->mSwapChain, 0, 0.3f, 0.4f, 1);

	vertex list[] = {
		{ Vec3(-.5f, -.5f, .0f) },
		{ Vec3(.0f, .5f, .0f) },
		{ Vec3(.5f, -.5f, .0f) },
	};

	//void* shader_byte_code = nullptr;
	//size_t size_shader = 0;
	//GraphicsEngine::get()->getRenderSystem()->compileShaderFromFile(L"shader.fx", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);

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
