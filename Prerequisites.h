#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>
#include <exception>

class GraphicsEngine;
class RenderSystem;
class SwapChain;
class DeviceContext;
class VertexBuffer;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
