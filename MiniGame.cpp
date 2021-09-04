#include "MiniGame.h"

MiniGame::MiniGame() = default;

MiniGame::~MiniGame() = default;

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

void MiniGame::onCreate()
{
	// Base material - to hold the directional light property for most meshes
	mBaseMat = GraphicsEngine::get()->createMaterial(
		L"DirectionalLightVS.hlsl", L"DirectionalLightPS.hlsl"
	);
	mBaseMat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	// Sky mesh
	mSkyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere_hq.obj");
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

	mRenderTarget = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(1280, 720), Texture::Type::RenderTarget);
	mDepthStencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(1280, 720), Texture::Type::DepthStencil);

	this->mProjCam.setPerspectiveFovLH(
		1.57f,
		(float)(1280.0f / 720.0f),
		0.1f,
		5000.0f
	);
}

void MiniGame::onUpdate()
{
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

	// Update camera & light
	this->updateThirdPersonCamera();
	this->updateLight();

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;

	m_forward = 0;
	m_rightward = 0;

	m_time += m_delta_time;
}

void MiniGame::onDestroy()
{
}

void MiniGame::onFocus()
{
}

void MiniGame::onKillFocus()
{
}

void MiniGame::onResize()
{
}

void MiniGame::onKeyDown(int key)
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

void MiniGame::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void MiniGame::onMouseMove(const Point & mouse_pos)
{
	int width = m_window_size.width;
	int height = m_window_size.height;

	m_delta_mouse_x = (int)(mouse_pos.m_x - (int)(m_window_size.left + (width / 2.0f)));
	m_delta_mouse_y = (int)(mouse_pos.m_y - (int)(m_window_size.top + (height / 2.0f)));
}

void MiniGame::onLeftMouseDown(const Point & mouse_pos)
{
}

void MiniGame::onLeftMouseUp(const Point & mouse_pos)
{
}

void MiniGame::onRightMouseDown(const Point & mouse_pos)
{
}

void MiniGame::onRightMouseUp(const Point & mouse_pos)
{
}

void MiniGame::setWindowSize(const Rect& size)
{
	this->m_window_size = size;
}

TexturePtr& MiniGame::getRenderTarget()
{
	return mRenderTarget;
}

void MiniGame::updateModel(Vec3 position, Vec3 rotation, Vec3 scale, const std::vector<MaterialPtr>&list_materials)
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

void MiniGame::updateThirdPersonCamera()
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

void MiniGame::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vec3(4000.0f, 4000.0f, 4000.0f));
	cc.m_world.setTranslation(mWorldCam.getTranslation());
	cc.m_view = mViewCam;
	cc.m_proj = mProjCam;

	mSkyMat->setData(&cc, sizeof(constant));
}

void MiniGame::updateLight()
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

void MiniGame::drawMesh(const MeshPtr & mesh, const std::vector<MaterialPtr>&list_materials)
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
