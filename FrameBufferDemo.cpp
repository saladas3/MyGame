#include "FrameBufferDemo.h"

FrameBufferDemo::FrameBufferDemo() = default;

FrameBufferDemo::~FrameBufferDemo() = default;

// Structure created to be passed through the constant buffer
// DirectX handles the constant data in video memory in chunks of 16B
//  so if we have a structure with 24B this size must be modified to be multiple of 16 (+ 8B)
__declspec(align(16)) // This line of code aligns into chuncks of 16B
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vec4 m_light_direction;
	Vec4 m_camera_position;
	Vec4 m_light_position = Vec4(0, 1, 0, 0);
	// Light radius is for point light
	float m_light_radius = 4.0f;
	ULONGLONG m_time = 0;
};

void FrameBufferDemo::onCreate()
{
	Window::onCreate();

	// Create the swap chain
	RECT rc = this->getClientWindowRect();
	mSwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->mHwnd, rc.right - rc.left, rc.bottom - rc.top
	);

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	// Base material - to hold the directional light property for most meshes
	mBaseMat = GraphicsEngine::get()->createMaterial(
		L"DirectionalLightVS.hlsl", L"DirectionalLightPS.hlsl"
	);
	mBaseMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	// Sky mesh
	mSkyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere_hq.obj");
	mSkyTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
	mSkyMat = GraphicsEngine::get()->createMaterial(L"SkyBoxVS.hlsl", L"SkyBoxPS.hlsl");
	mSkyMat->addTexture(mSkyTex);
	mSkyMat->setCullMode(CULL_MODE::CULL_MODE_FRONT);

	// Monitor mesh
	mMonitorMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\monitor.obj");

	mMonitorTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick_d.jpg");
	mScreenTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");

	mMonitorMat = GraphicsEngine::get()->createMaterial(mBaseMat);
	mMonitorMat->addTexture(mMonitorTex);
	mMonitorMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	mScreenMat = GraphicsEngine::get()->createMaterial(mBaseMat);
	mScreenMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	m_mini_game.setWindowSize(Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top));
	m_mini_game.onCreate();

	mScreenMat->addTexture(m_mini_game.getRenderTarget());
}

void FrameBufferDemo::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	m_mini_game.onUpdate();

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(
		this->mSwapChain, 0, 0.3f, 0.4f, 1
	);

	// Set viewport of render target in which we have to draw (the whole window)
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(
		rc.right - rc.left, rc.bottom - rc.top
	);

	// Render skybox
	m_list_materials.clear();
	m_list_materials.push_back(mSkyMat);
	this->drawMesh(mSkyMesh, m_list_materials);
	this->updateSkyBox();

	// Render monitor mesh
	m_list_materials.clear();
	// The order in which the materials are set is important
	m_list_materials.push_back(mMonitorMat);
	m_list_materials.push_back(mScreenMat);
	this->drawMesh(mMonitorMesh, m_list_materials);
	this->updateModel(Vec3(0, 0, 2), Vec3(0, 3.14f, 0), Vec3(1, 1, 1), m_list_materials);

	// Update camera & light
	this->updateThirdPersonCamera();
	this->updateLight();

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;

	m_forward = 0;
	m_rightward = 0;

	m_time += m_delta_time;

	// Start swapping between the back and front buffer and present the rendered images on screen
	this->mSwapChain->present(true);
}

void FrameBufferDemo::onDestroy()
{
	Window::onDestroy();
}

void FrameBufferDemo::onFocus()
{
	Window::onFocus();
	InputSystem::get()->addListener(this);
}

void FrameBufferDemo::onKillFocus()
{
	Window::onKillFocus();
	InputSystem::get()->removeListener(this);
}

void FrameBufferDemo::onResize()
{
	Window::onResize();
	RECT rc = this->getClientWindowRect();
	this->mSwapChain->resize(rc.right - rc.left, rc.bottom - rc.top);

	m_mini_game.setWindowSize(Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top));
}

void FrameBufferDemo::onKeyDown(int key)
{
	m_mini_game.onKeyDown(key);
}

void FrameBufferDemo::onKeyUp(int key)
{
	m_mini_game.onKeyUp(key);

	if (key == 'F') {
		// Toggle fullscreen
		mFullScreen = (mFullScreen) ? false : true;
		RECT size_screen = this->getSizeScreen();
		this->mSwapChain->setFullScreen(mFullScreen, size_screen.right, size_screen.bottom);
	}
}

void FrameBufferDemo::onMouseMove(const Point & mouse_pos)
{
	m_mini_game.onMouseMove(mouse_pos);
	RECT win_size = this->getClientWindowRect();

	int width = win_size.right - win_size.left;
	int height = win_size.bottom - win_size.top;

	// Keep the mouse in the center of the screen
	InputSystem::get()->setCursorPosition(Point(win_size.left + width / 2.0f, win_size.top + height / 2.0f));
}

void FrameBufferDemo::onLeftMouseDown(const Point & mouse_pos)
{
}

void FrameBufferDemo::onLeftMouseUp(const Point & mouse_pos)
{
}

void FrameBufferDemo::onRightMouseDown(const Point & mouse_pos)
{
}

void FrameBufferDemo::onRightMouseUp(const Point & mouse_pos)
{
}

void FrameBufferDemo::updateModel(Vec3 position, Vec3 rotation, Vec3 scale, const std::vector<MaterialPtr>&list_materials)
{
	constant cc;
	Matrix4x4 temp;

	cc.m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(rotation.m_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(rotation.m_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationZ(rotation.m_z);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setTranslation(position);
	cc.m_world *= temp;

	cc.m_view = mViewCam;
	cc.m_proj = mProjCam;
	cc.m_camera_position = mWorldCam.getTranslation();

	cc.m_light_position = mLightPosition;
	cc.m_light_direction = mLightRotMatrix.getZDirection();
	cc.m_time = m_time;

	for (size_t m = 0; m < list_materials.size(); m++)
	{
		list_materials[m]->setData(&cc, sizeof(constant));
	}
}

void FrameBufferDemo::updateThirdPersonCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_cam_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	// Make the camera to not be able to do 360 degrees spin on the x axis
	if (m_cam_rotation.m_x >= 1.57f)
		m_cam_rotation.m_x = 1.57f;
	else if (m_cam_rotation.m_x <= -1.57f)
		m_cam_rotation.m_x = -1.57f;

	// Smooth camera roation animation
	//m_current_cam_rotation = Vec3::lerp(m_current_cam_rotation, m_cam_rotation, 3.0f * m_delta_time);
	// No animation
	m_current_cam_rotation = m_cam_rotation;

	temp.setIdentity();
	temp.setRotationX(m_current_cam_rotation.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_cam_rotation.m_y);
	world_cam *= temp;

	// Smooth camera animation
	//m_current_cam_distance = lerp(m_current_cam_distance, m_cam_distance, 2.0f * m_delta_time);
	// No animation
	//m_current_cam_distance = m_cam_distance;

	m_cam_position = (
		m_cam_position +
		world_cam.getZDirection() * (m_forward) * 15.0f * m_delta_time +
		world_cam.getXDirection() * (m_rightward) * 15.0f * m_delta_time
		);

	// Used to set the camera behind the player and up a bit
	//Vec3 new_pos = m_cam_position + world_cam.getZDirection() * (-m_current_cam_distance);
	// Move the camera behind the player a bit on the Y axis (up)
	//new_pos = new_pos + world_cam.getYDirection() * (1.4f);

	world_cam.setTranslation(m_cam_position);

	this->mWorldCam = world_cam;

	// Make the camera matrix a view matrix -> invert it
	world_cam.inverse();

	this->mViewCam = world_cam;

	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	this->mProjCam.setPerspectiveFovLH(
		1.57f,
		(float)((float)width / (float)height),
		0.1f,
		5000.0f
	);
}

void FrameBufferDemo::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vec3(4000.0f, 4000.0f, 4000.0f));
	cc.m_world.setTranslation(mWorldCam.getTranslation());
	cc.m_view = mViewCam;
	cc.m_proj = mProjCam;

	mSkyMat->setData(&cc, sizeof(constant));
}

void FrameBufferDemo::updateLight()
{
	Matrix4x4 temp;

	mLightRotMatrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(-0.707f);
	mLightRotMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(2.14f);
	mLightRotMatrix *= temp;
}

void FrameBufferDemo::drawMesh(const MeshPtr & mesh, const std::vector<MaterialPtr>&list_materials)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{
		if (m >= list_materials.size()) break;
		MaterialSlot mat = mesh->getMaterialSlot(m);
		GraphicsEngine::get()->setMaterial(list_materials[m]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.NumIndices, 0, mat.StartIndex);
	}
}
