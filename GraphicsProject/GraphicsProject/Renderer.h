#pragma once
#include "GraphicsStructs.h"

using namespace DirectX;
class RenderObject;
class Camera;
class RenderList;
struct cb_DEFAULT;

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class Renderer
{
private:
	static UINT resolutionWidth;
	static UINT resolutionHeight;
	static ID3D11ShaderResourceView *theDepthStencilSRVPtr;

	static RenderObject* testOBJ;

	/// Interface to add and render all objects
	static RenderList* m_pRenderList;

	static ID3D11Debug* m_pDebug;
public:
	Renderer() {};
	~Renderer() {};

	static ID3D11Buffer* constantBuffer;
	static cb_DEFAULT* toShader;
	/// Interface for feature support queries and resource allocation
	static ID3D11Device* theDevicePtr;
	/// Interface to rendering commands, close relation to D3D9 device interface
	static ID3D11DeviceContext* theContextPtr;
	/// Interface for storing rendered data and presenting
	static IDXGISwapChain* theSwapChainPtr;
	/// Interface view to the back buffer
	static ID3D11RenderTargetView* theRenderTargetViewPtr;
	/// Interface to the texture representing our back buffer
	static ID3D11Texture2D*theBackBufferPtr;
	/// Interface to the texture representing the depth buffer
	static ID3D11Texture2D* theDepthStencilBufferPtr;
	/// Interface view to the depth buffer
	static ID3D11DepthStencilView* theDepthStencilViewPtr;
	/// Defines the demensions of the viewport we render to
	static D3D11_VIEWPORT theScreenViewport;
	/// Defines how rasterizer stage should behave default
	static ID3D11RasterizerState* theDefaultRasterState;
	/// Interface that defines how vertex daa is layed out
	static ID3D11InputLayout* theDefaultInputLayout;
	/// Camera for debug purposes
	static Camera* MainCamera;
	/// Defines how a texture will be sampled
	static ID3D11SamplerState* theSamplerState;
	
	static void Initialize(HWND hWnd, UINT resWidth, UINT resHeight);
	static void SetResolution(UINT _width, UINT _height);
	static void Shutdown();
	static void ResizeBuffers();
	static void Render(float deltaTime);
};

