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

	// Create the swap chain
	RECT rc = this->getClientWindowRect();
	mSwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->mHwnd, rc.right - rc.left, rc.bottom - rc.top);

	// ------------------------------------------------------
	// Temp - used for testing; DELETE AFTER
	// ------------------------------------------------------
	vertex list[] = {
		{ Vec3(-.5f, -.5f, .0f) },
		{ Vec3(.0f, .5f, .0f) },
		{ Vec3(.5f, -.5f, .0f) },
	};

	void* sbc1 = nullptr;
	size_t ss1 = 0;
	void* sbc2 = nullptr;
	size_t ss2 = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"shader.fx", "vsmain", &sbc1, &ss1);
	mTempVertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(&sbc1, ss1);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"shader.fx", "psmain", &sbc2, &ss2);
	mTempPixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(&sbc2, ss2);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	mTempVertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(list, sizeof(vertex), ARRAYSIZE(list), sbc1, ss1);
	// ------------------------------------------------------
}

void MainGame::onUpdate()
{
	Window::onUpdate();

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->mSwapChain, 0, 0.3f, 0.4f, 1);

	// Set viewport of render target in which we have to draw (the whole screen)
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	
	// Temp - used for testing; DELETE AFTER
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(mTempVertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(mTempPixelShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mTempVertexBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawTriangleList(mTempVertexBuffer->getSizeVertexList(), 0);

	// Start swapping between the back and front buffer and present the rendered images on screen
	this->mSwapChain->present(true);
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
