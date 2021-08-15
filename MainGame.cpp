#include "MainGame.h"

MainGame::MainGame() = default;

MainGame::~MainGame() = default;

// ------------------------------------------------------
// Temp - used for testing; DELETE AFTER
// ------------------------------------------------------
struct vertex
{
	Vec3 position;
	Vec3 color;
	Vec3 color1;
};
// ------------------------------------------------------

// Structure created to be passed through the constant buffer
// DirectX handles the constant data in video memory in chunks of 16B
//  so if we have a structure with 24B this size must be modified to be multiple of 16 (+ 8B)
__declspec(align(16)) // This line of code aligns into chuncks of 16B
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	ULONGLONG m_time = 0;
};

void MainGame::onCreate()
{
	Window::onCreate();

	// Create the swap chain
	RECT rc = this->getClientWindowRect();
	mSwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->mHwnd, rc.right - rc.left, rc.bottom - rc.top);

	InputSystem::get()->addListener(this);
	//InputSystem::get()->showCursor(false);

	// ------------------------------------------------------
	// Temp - used for testing; DELETE AFTER
	// ------------------------------------------------------
	vertex list[] = {
		// Front face of the cube
		{ Vec3(-0.5f,-0.5f,-0.5f),	Vec3(1,0,0),	Vec3(0.2f,0,0)		},
		{ Vec3(-0.5f,0.5f,-0.5f),	Vec3(1,1,0),	Vec3(0.2f,0.2f,0)	},
		{ Vec3(0.5f,0.5f,-0.5f),	Vec3(1,1,0),	Vec3(0.2f,0.2f,0)	},
		{ Vec3(0.5f,-0.5f,-0.5f),	Vec3(1,0,0),	Vec3(0.2f,0,0)		},

		// Back face of the cube
		{ Vec3(0.5f,-0.5f,0.5f),	Vec3(0,1,0),	Vec3(0,0.2f,0)		},
		{ Vec3(0.5f,0.5f,0.5f),		Vec3(0,1,1),	Vec3(0,0.2f,0.2f)	},
		{ Vec3(-0.5f,0.5f,0.5f),	Vec3(0,1,1),	Vec3(0,0.2f,0.2f)	},
		{ Vec3(-0.5f,-0.5f,0.5f),	Vec3(0,1,0),	Vec3(0,0.2f,0)		}
	};

	unsigned int index_list[] = {
		// FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		// BACK SIDE
		4,5,6,
		6,7,4,
		// TOP SIDE
		1,6,5,
		5,2,1,
		// BOTTOM SIDE
		7,0,3,
		3,4,7,
		// RIGHT SIDE
		3,2,5,
		5,4,3,
		// LEFT SIDE
		7,6,1,
		1,0,7
	};

	mTempIndexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, ARRAYSIZE(index_list));

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
	mTempConstantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	// ------------------------------------------------------
}

void MainGame::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->mSwapChain, 0, 0.3f, 0.4f, 1);

	// Set viewport of render target in which we have to draw (the whole window)
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	
	// ------------------------------------------------------
	// Temp - used for testing; DELETE AFTER
	// ------------------------------------------------------
	this->testMethod();

	mOldDelta = mNewDelta;
	mNewDelta = ::GetTickCount64();
	mDeltaTime = (mOldDelta) ? (mNewDelta - mOldDelta) / 1000.0f : 0;
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
	InputSystem::get()->addListener(this);
}

void MainGame::onKillFocus()
{
	Window::onKillFocus();
	InputSystem::get()->removeListener(this);
}

void MainGame::onResize()
{
	Window::onResize();
	RECT rc = this->getClientWindowRect();
	mSwapChain->resize(rc.right - rc.left, rc.bottom - rc.top);
}

void MainGame::onKeyDown(int key)
{
	switch (key)
	{
		case 'W':
		{
			mRotX += mDeltaTime * 3;
			break;
		}
		case 'S':
		{
			mRotX -= mDeltaTime * 3;
			break;
		}
		case 'A':
		{
			mRotY += mDeltaTime * 3;
			break;
		}
		case 'D':
		{
			mRotY -= mDeltaTime * 3;
			break;
		}
		default:
			break;
	}
}

void MainGame::onKeyUp(int key)
{
}

void MainGame::onMouseMove(const Point& mouse_pos)
{
	mRotX -= mDeltaTime * mouse_pos.m_y / 500.0f;
	mRotY -= mDeltaTime * mouse_pos.m_x / 500.0f;
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

void MainGame::testMethod()
{
	Matrix4x4 tempMatrix;

	constant cc;
	cc.m_time = ::GetTickCount64(); // Get the time elapsed since the system was started in miliseconds

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vec3(1, 1, 1));

	// Set rotation Y
	tempMatrix.setIdentity();
	tempMatrix.setRotationY(mRotY);
	cc.m_world *= tempMatrix;

	// Set rotation X
	tempMatrix.setIdentity();
	tempMatrix.setRotationX(mRotX);
	cc.m_world *= tempMatrix;

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);

	// Update the constant buffer with new values
	mTempConstantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	// Set the new constant buffer after it's been updated (for both pixel and vertex shader)
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mTempVertexShader, mTempConstantBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mTempPixelShader, mTempConstantBuffer);

	// Set vertex and pixel shader
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(mTempVertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(mTempPixelShader);

	// Set Vertex Buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mTempVertexBuffer);

	// Set Index Buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mTempIndexBuffer);

	// Draw the vertices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mTempIndexBuffer->getSizeIndexList(), 0, 0);
}
