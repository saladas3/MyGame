#include "MainGame.h"

MainGame::MainGame() = default;

MainGame::~MainGame() = default;

// ------------------------------------------------------
// Temp - used for testing; DELETE AFTER
// ------------------------------------------------------
struct vertex {
	Vec3 position;
	Vec3 position1;
	Vec3 color;
	Vec3 color1;
};

// Structured created to be passed through the constant buffer
// DirectX handles the constant data in video memory in chunks of 16B
//  so if we have a structure with 24B this size must be modified to be multiple of 16 (+ 8B)
__declspec(align(16)) // This line of code aligns into chuncks of 16B
struct constant {
	unsigned int m_time;
};
// ------------------------------------------------------

void MainGame::onCreate()
{
	Window::onCreate();

	// Create the swap chain
	RECT rc = this->getClientWindowRect();
	mSwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->mHwnd, rc.right - rc.left, rc.bottom - rc.top);

	// ------------------------------------------------------
	// Temp - used for testing; DELETE AFTER
	// ------------------------------------------------------
	// 4 vertices - their information is stored in vertex buffer
	vertex list[] = {
		{ Vec3(-0.5f,-0.5f,0.0f), Vec3(-0.32f,-0.11f,0.0f), Vec3(0,0,0), Vec3(0,1,0) },
		{ Vec3(-0.5f,0.5f,0.0f), Vec3(-0.11f,0.78f,0.0f), Vec3(1,1,0), Vec3(0,1,1) },
		{ Vec3(0.5f,-0.5f,0.0f), Vec3(0.75f,-0.73f,0.0f), Vec3(0,0,1), Vec3(1,0,0) },
		{ Vec3(0.5f,0.5f,0.0f), Vec3(0.88f,0.77f,0.0f), Vec3(1,1,1), Vec3(0,0,1) }
	};

	void* sbc1 = nullptr;
	size_t ss1 = 0;
	
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VShader.hlsl", "vsmain", &sbc1, &ss1);
	mTempVertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(sbc1, ss1);
	
	// Need to create the vertex buffer before releasing the compiled vertex shader buffer
	mTempVertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(list, sizeof(vertex), ARRAYSIZE(list), sbc1, ss1);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	void* sbc2 = nullptr;
	size_t ss2 = 0;

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PShader.hlsl", "psmain", &sbc2, &ss2);
	mTempPixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(sbc2, ss2);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;
	mTempConstantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
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
	
	// ------------------------------------------------------
	// Temp - used for testing; DELETE AFTER
	// ------------------------------------------------------
	// Continuosly update the constant buffer with new values
	constant cc;
	cc.m_time = ::GetTickCount64(); // Get the time elapsed since the system was started in miliseconds
	mTempConstantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mTempVertexShader, mTempConstantBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mTempPixelShader, mTempConstantBuffer);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(mTempVertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(mTempPixelShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mTempVertexBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawTriangleStrip(mTempVertexBuffer->getSizeVertexList(), 0);
	// ------------------------------------------------------

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
