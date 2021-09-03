#pragma once

#include "Window.h"
#include "Matrix4x4.h"
#include "InputListener.h"
#include "InputSystem.h"
#include "MathUtils.h"
#include "GraphicsEngine.h"

#include <iostream>

class MiniGame
{

public:
    MiniGame();
    ~MiniGame();

public:
    // Inherited via Window
    void onCreate();
    void onUpdate();
    void onDestroy();
    void onFocus();
    void onKillFocus();
    void onResize();

    // Inherited via InputListener
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual void onMouseMove(const Point& mouse_pos);
    virtual void onLeftMouseDown(const Point& mouse_pos);
    virtual void onLeftMouseUp(const Point& mouse_pos);
    virtual void onRightMouseDown(const Point& mouse_pos);
    virtual void onRightMouseUp(const Point& mouse_pos);

    void setWindowSize(const Rect& size);
    TexturePtr& getRenderTarget();

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

    TexturePtr mRenderTarget = nullptr;
    TexturePtr mDepthStencil = nullptr;

    Rect m_window_size;

    /* --------------------------------------------------- */

    std::vector<MaterialPtr> m_list_materials;
    // ------------------------------------------------------

private:
    SwapChainPtr mSwapChain = nullptr;

};

