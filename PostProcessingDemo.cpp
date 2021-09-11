#include "PostProcessingDemo.h"

// NOTE(luca) - By rendering the game not on the screen but on a quad that covers the screen leaves room to apply any kind of effects (ex: distortion)

PostProcessingDemo::PostProcessingDemo() = default;

PostProcessingDemo::~PostProcessingDemo() = default;

// NOTE(luca) - To send data from the constant buffer to the pixel shader where the effects are created we must use: mPostProcessMat.setData(<new_c_buffer>)

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

void PostProcessingDemo::onCreate()
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
	mSkyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	mSkyTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	mSkyMat = GraphicsEngine::get()->createMaterial(L"SkyBoxVS.hlsl", L"SkyBoxPS.hlsl");
	mSkyMat->addTexture(mSkyTex);
	mSkyMat->setCullMode(CULL_MODE::CULL_MODE_FRONT);

	// Player mesh
	mPlayerMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\player.obj");
	mPlayerTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sand.jpg");
	mPlayerMat = GraphicsEngine::get()->createMaterial(mBaseMat);
	mPlayerMat->addTexture(mPlayerTex);
	mPlayerMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	// BumpMapping test
	mSphereMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	mSphereTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick_d.jpg");
	mSphereNTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick_n.jpg");
	mSphereMat = GraphicsEngine::get()->createMaterial(L"BumpMappingVS.hlsl", L"BumpMappingPS.hlsl");
	mSphereMat->addTexture(mSphereTex);
	mSphereMat->addTexture(mSphereNTex);
	mSphereMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	// Base material - to hold the directional light property for most meshes
	mPostProcessMat = GraphicsEngine::get()->createMaterial(
		L"PostProcessVS.hlsl", L"DistortionEffectPS.hlsl"
	);
	mPostProcessMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	VertexMesh quad_vertex_list[] = {
		VertexMesh(
			Vec3(-1, -1, 0), Vec2(0, 1), Vec3(), Vec3(), Vec3()
		),
		VertexMesh(
			Vec3(-1,  1, 0), Vec2(0, 0), Vec3(), Vec3(), Vec3()
		),
		VertexMesh(
			Vec3( 1,  1, 0), Vec2(1, 0), Vec3(), Vec3(), Vec3()
		),
		VertexMesh(
			Vec3( 1, -1, 0), Vec2(1, 1), Vec3(), Vec3(), Vec3()
		),
	};

	UINT quad_index_list[] = {
		0, 1, 2,
		2, 3, 0,
	};

	MaterialSlot quad_mat_slots[] = {
		{0, 6, 0}
	};

	// Create the quad in which we render the game
	this->mQuadMesh = GraphicsEngine::get()->getMeshManager()->createMesh(
		quad_vertex_list, 4,
		quad_index_list, 6,
		quad_mat_slots, 1
	);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	this->mProjCam.setPerspectiveFovLH(
		1.57f,
		(float)((float)width / (float)height),
		0.1f,
		5000.0f
	);

	m_list_materials.reserve(32);

	mRenderTarget = GraphicsEngine::get()->getTextureManager()->createTexture(
		Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::RenderTarget
	);
	mDepthStencil = GraphicsEngine::get()->getTextureManager()->createTexture(
		Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::DepthStencil
	);

	mPostProcessMat->addTexture(mRenderTarget);
}

void PostProcessingDemo::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	// ------------------------------------------------
	// SCENE RENDERED TO RENDER TARGET
	// ------------------------------------------------

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(
		this->mRenderTarget, 0, 0.3f, 0.4f, 1
	);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(
		this->mDepthStencil
	);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(
		this->mRenderTarget, this->mDepthStencil
	);

	// Set viewport of render target in which we have to draw (the whole window)
	Rect viewport_size = mRenderTarget->getSize();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(
		viewport_size.width, viewport_size.height
	);

	// Render skybox
	m_list_materials.clear();
	m_list_materials.push_back(mSkyMat);
	this->drawMesh(mSkyMesh, m_list_materials);
	this->updateSkyBox();

	// Render player mesh
	m_list_materials.clear();
	m_list_materials.push_back(mPlayerMat);
	this->drawMesh(mPlayerMesh, m_list_materials);
	this->updateModel(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1), m_list_materials);

	// Normal Mapping Render technique test
	m_list_materials.clear();
	m_list_materials.push_back(mSphereMat);
	this->drawMesh(mSphereMesh, m_list_materials);
	this->updateModel(Vec3(3, 1, 0), Vec3(0, 0, 0), Vec3(1, 1, 1), m_list_materials);

	// ------------------------------------------------

	// Update camera & light
	this->updateThirdPersonCamera();
	this->updateLight();

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;

	m_forward = 0;
	m_rightward = 0;

	m_time += m_delta_time;

	// Clear the whole window and show a solid color
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(
		this->mSwapChain, 0, 0.3f, 0.4f, 1
	);

	// Set viewport of render target in which we have to draw (the whole window)
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(
		rc.right - rc.left, rc.bottom - rc.top
	);

	m_list_materials.clear();
	m_list_materials.push_back(mPostProcessMat);
	this->drawMesh(mQuadMesh, m_list_materials);

	// Start swapping between the back and front buffer and present the rendered images on screen
	this->mSwapChain->present(true);
}

void PostProcessingDemo::onDestroy()
{
	Window::onDestroy();
}

void PostProcessingDemo::onFocus()
{
	Window::onFocus();
	InputSystem::get()->addListener(this);
}

void PostProcessingDemo::onKillFocus()
{
	Window::onKillFocus();
	InputSystem::get()->removeListener(this);
}

void PostProcessingDemo::onResize()
{
	Window::onResize();
	RECT rc = this->getClientWindowRect();
	this->mSwapChain->resize(rc.right - rc.left, rc.bottom - rc.top);

	mRenderTarget = GraphicsEngine::get()->getTextureManager()->createTexture(
		Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::RenderTarget
	);
	mDepthStencil = GraphicsEngine::get()->getTextureManager()->createTexture(
		Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::DepthStencil
	);

	mPostProcessMat->removeTexture(0);
	mPostProcessMat->addTexture(mRenderTarget);
}

void PostProcessingDemo::onKeyDown(int key)
{
	switch (key)
	{
	case 'W':
	{
		m_forward = 1.0f;
		break;
	}
	case 'S':
	{
		m_forward = -1.0f;
		break;
	}
	case 'A':
	{
		m_rightward = -1.0f;
		break;
	}
	case 'D':
	{
		m_rightward = 1.0f;
		break;
	}
	default:
		break;
	}
}

void PostProcessingDemo::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'F') {
		// Toggle fullscreen
		mFullScreen = (mFullScreen) ? false : true;
		RECT size_screen = this->getSizeScreen();
		this->mSwapChain->setFullScreen(mFullScreen, size_screen.right, size_screen.bottom);
	}
}

void PostProcessingDemo::onMouseMove(const Point & mouse_pos)
{
	RECT win_size = this->getClientWindowRect();

	int width = win_size.right - win_size.left;
	int height = win_size.bottom - win_size.top;

	m_delta_mouse_x = (int)(mouse_pos.m_x - (int)(win_size.left + (width / 2.0f)));
	m_delta_mouse_y = (int)(mouse_pos.m_y - (int)(win_size.top + (height / 2.0f)));

	// Keep the mouse in the center of the screen
	InputSystem::get()->setCursorPosition(Point(win_size.left + width / 2.0f, win_size.top + height / 2.0f));
}

void PostProcessingDemo::onLeftMouseDown(const Point & mouse_pos)
{
}

void PostProcessingDemo::onLeftMouseUp(const Point & mouse_pos)
{
}

void PostProcessingDemo::onRightMouseDown(const Point & mouse_pos)
{
}

void PostProcessingDemo::onRightMouseUp(const Point & mouse_pos)
{
}

void PostProcessingDemo::updateModel(Vec3 position, Vec3 rotation, Vec3 scale, const std::vector<MaterialPtr>&list_materials)
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

void PostProcessingDemo::updateThirdPersonCamera()
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
}

void PostProcessingDemo::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vec3(4000.0f, 4000.0f, 4000.0f));
	cc.m_world.setTranslation(mWorldCam.getTranslation());
	cc.m_view = mViewCam;
	cc.m_proj = mProjCam;

	mSkyMat->setData(&cc, sizeof(constant));
}

void PostProcessingDemo::updateLight()
{
	Matrix4x4 temp;

	mLightRotMatrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(-0.707f);
	mLightRotMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(-m_time * .5f);
	mLightRotMatrix *= temp;
}

void PostProcessingDemo::drawMesh(const MeshPtr & mesh, const std::vector<MaterialPtr>&list_materials)
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
