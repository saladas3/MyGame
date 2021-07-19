#pragma once

#include "Window.h"
#include "Matrix4x4.h"
#include "InputListener.h"
#include "GraphicsEngine.h"

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
    // ------------------------------------------------------
    // Temp - used for testing; DELETE AFTER
    // ------------------------------------------------------
    void testMethod();

    float mOldDelta = 0; // Time point when the previous frame was rendered
    float mNewDelta = 0; // Time point when the current frame was rendered
    float mDeltaTime = 0; // Difference between the 2 above

    float mDeltaPos = 0;

    float mDeltaScale = 0;

    VertexBufferPtr mTempVertexBuffer = nullptr;
    VertexShaderPtr mTempVertexShader = nullptr;
    PixelShaderPtr mTempPixelShader = nullptr;
    ConstantBufferPtr mTempConstantBuffer = nullptr;
    // ------------------------------------------------------

private:
    SwapChainPtr mSwapChain = nullptr;

};

