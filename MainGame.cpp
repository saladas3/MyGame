#include "MainGame.h"

MainGame::MainGame() = default;

MainGame::~MainGame() = default;

// ------------------------------------------------------
// Temp - used for testing; DELETE AFTER
// ------------------------------------------------------
struct vertex
{
	Vec3 position;
	Vec2 texcoord;
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
	InputSystem::get()->showCursor(false);

	// ------------------------------------------------------
	// Temp - used for testing; DELETE AFTER
	// ------------------------------------------------------

	mWoodTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wood.jpg");

	mWorldCam.setTranslation(Vec3(0, 0, -2));

	Vec3 position_list[] =
	{
		{ Vec3(-0.5f,-0.5f,-0.5f)},
		{ Vec3(-0.5f,0.5f,-0.5f) },
		{ Vec3(0.5f,0.5f,-0.5f) },
		{ Vec3(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vec3(0.5f,-0.5f,0.5f) },
		{ Vec3(0.5f,0.5f,0.5f) },
		{ Vec3(-0.5f,0.5f,0.5f)},
		{ Vec3(-0.5f,-0.5f,0.5f) }
	};

	Vec2 texcoord_list[] =
	{
		{ Vec2(0.0f,0.0f) },
		{ Vec2(0.0f,1.0f) },
		{ Vec2(1.0f,0.0f) },
		{ Vec2(1.0f,1.0f) }
	};

	vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },


		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },


		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }

	};

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};


	mTempIndexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, ARRAYSIZE(index_list));

	void* sbc1 = nullptr;
	size_t ss1 = 0;
	
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VShader.hlsl", "vsmain", &sbc1, &ss1);
	mTempVertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(sbc1, ss1);
	
	// Need to create the vertex buffer before releasing the compiled vertex shader buffer
	mTempVertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), ARRAYSIZE(vertex_list), sbc1, ss1);

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
			mForward = 1.0f;
			break;
		}
		case 'S':
		{
			mForward = -1.0f;
			break;
		}
		case 'A':
		{
			mRightward = -1.0f;
			break;
		}
		case 'D':
		{
			mRightward = 1.0f;
			break;
		}
		default:
			break;
	}
}

void MainGame::onKeyUp(int key)
{
	mForward = 0.0f;
	mRightward = 0.0f;
}

void MainGame::onMouseMove(const Point& mouse_pos)
{
	RECT win_size = this->getClientWindowRect();

	int width = win_size.right - win_size.left;
	int height = win_size.bottom - win_size.top;

	mRotX += (mouse_pos.m_y - (height / 2.0f)) * mDeltaTime * .3f;
	mRotY += (mouse_pos.m_x - (width / 2.0f)) * mDeltaTime * .3f;
	
	// Keep the mouse in the center of the screen
	//InputSystem::get()->setCursorPosition(Point(win_size.left + width / 2.0f, win_size.top + height / 2.0f));
	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
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
	/* ----- Camera and movement ----- */

	Matrix4x4 tempMatrix, worldCam;
	worldCam.setIdentity();

	tempMatrix.setIdentity();
	tempMatrix.setRotationX(mRotX);
	worldCam *= tempMatrix;

	tempMatrix.setIdentity();
	tempMatrix.setRotationY(mRotY);
	worldCam *= tempMatrix;

	Vec3 new_pos = mWorldCam.getTranslation() + worldCam.getZDirection() * mForward * .3f;
	new_pos = new_pos + worldCam.getXDirection() * mRightward * .3f;
	worldCam.setTranslation(new_pos);
	mWorldCam = worldCam;

	// Make the camera matrix a view matrix by inverting it
	worldCam.inverse();

	constant cc;
	cc.m_time = ::GetTickCount64(); // Get the time elapsed since the system was started in miliseconds
	cc.m_world.setIdentity();
	cc.m_view = worldCam;

	LONG width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	LONG height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	// 1.57f -> pi/2 -> field of view angle (it's wide)
	cc.m_proj.setPerspectiveFovLH(1.57f, (float)width / (float)height, .1f, 100.0f);

	/* ----- /Camera and movement ----- */

	// Update the constant buffer with new values
	mTempConstantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	// Set the new constant buffer after it's been updated (for both pixel and vertex shader)
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mTempVertexShader, mTempConstantBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mTempPixelShader, mTempConstantBuffer);

	// Set vertex and pixel shader
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(mTempVertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(mTempPixelShader);

	// Set texture
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(mTempVertexShader, &mWoodTex, 1);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(mTempPixelShader, &mWoodTex, 1);

	// Set Vertex Buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mTempVertexBuffer);

	// Set Index Buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mTempIndexBuffer);

	// Draw the vertices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mTempIndexBuffer->getSizeIndexList(), 0, 0);

	mOldDelta = mNewDelta;
	mNewDelta = ::GetTickCount64();
	mDeltaTime = (mOldDelta) ? (mNewDelta - mOldDelta) / 1000.0f : 0;
}
