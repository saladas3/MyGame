#pragma once

#include "Window.h"
#include "InputListener.h"

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

};

