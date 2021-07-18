#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

// -----------------------------------------
// Constant buffer is a buffer in the video memory in which we save things like:
// Vec3 Position;
// Matrix world;
// Matrix view;
// Matrix projection;
// Float color;
// Unsigned integer Time; etc.
// So basically it's a way to communicate with the shader files.
// -----------------------------------------

class ConstantBuffer
{

public:
    ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);
    ~ConstantBuffer();

public:
    void update(const DeviceContextPtr& context, void* buffer);

private:
    ID3D11Buffer* mBuffer = nullptr;
    RenderSystem* mRenderSystem = nullptr;

private:
    friend class DeviceContext;

};

