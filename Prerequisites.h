#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>

class GraphicsEngine;
class RenderSystem;
class SwapChain;
class DeviceContext;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
