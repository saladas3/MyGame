#pragma once

#include "Window.h"
#include "Matrix4x4.h"
#include "InputListener.h"
#include "InputSystem.h"
#include "MathUtils.h"
#include "GraphicsEngine.h"

#include <iostream>

class MainGame : public Window, public InputListener
{

public:
	MainGame();
	~MainGame();

public:
    // Inherited via Window
    void onCreate() override;
    void onUpdate() override;
    void onDestroy() override;
    void onFocus() override;
    void onKillFocus() override;
    void onResize() override;

    // Inherited via InputListener
    virtual void onKeyDown(int key) override;
    virtual void onKeyUp(int key) override;
    virtual void onMouseMove(const Point& mouse_pos) override;
    virtual void onLeftMouseDown(const Point& mouse_pos) override;
    virtual void onLeftMouseUp(const Point& mouse_pos) override;
    virtual void onRightMouseDown(const Point& mouse_pos) override;
    virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
    ConstantBufferPtr mCtBuffer = nullptr;
    bool mFullScreen = false;
    
    // ------------------------------------------------------
    // Temp - used for testing; DELETE AFTER
    // ------------------------------------------------------
    void updateModel(Vec3 position, Vec3 rotation, Vec3 scale, const std::vector<MaterialPtr>& list_materials);
    void updateThirdPersonCamera();
    void updateSkyBox();
    void updateLight();
    void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials);

    // Speed of the rendered frames
    float m_delta_time = 1.0f / 60.0f;
    float m_time = 0.0f;

    /* ------------- Camera & Player related ------------- */
    Vec3 m_current_cam_rotation;
    Vec3 m_cam_rotation;
    Vec3 m_cam_position;

    float m_current_cam_distance = 3.0f;
    float m_cam_distance = 3.0f;
    float m_delta_mouse_x = 0.0f;
    float m_delta_mouse_y = 0.0f;
    float m_forward = 0;
    float m_rightward = 0;

    bool m_player_sprint = false;

    Matrix4x4 mWorldCam;
    Matrix4x4 mViewCam;
    Matrix4x4 mProjCam;
    Matrix4x4 mLightRotMatrix;

    Vec4 mLightPosition;

    MaterialPtr mBaseMat = nullptr;

    MeshPtr mSkyMesh = nullptr;
    TexturePtr mSkyTex = nullptr;
    MaterialPtr mSkyMat = nullptr;

    MeshPtr mPlayerMesh = nullptr;
    TexturePtr mPlayerTex = nullptr;
    MaterialPtr mPlayerMat = nullptr;

    MeshPtr mSphereMesh = nullptr;
    TexturePtr mSphereTex = nullptr;
    TexturePtr mSphereNTex = nullptr;
    MaterialPtr mSphereMat = nullptr;
    
    /* --------------------------------------------------- */

    std::vector<MaterialPtr> m_list_materials;
    // ------------------------------------------------------

private:
    SwapChainPtr mSwapChain = nullptr;

};

