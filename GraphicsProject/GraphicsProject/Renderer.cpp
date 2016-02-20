#include "Renderer.h"
#include "Shaders.h"
#include "RenderObject.h"
#include "BufferManager.h"
#include "Camera.h"
#include "ConstantBuffers.h"
#include "RenderList.h"

using namespace std;
using namespace DirectX;

ID3D11Device* Renderer::theDevicePtr = 0;
ID3D11DeviceContext* Renderer::theContextPtr = 0;
IDXGISwapChain* Renderer::theSwapChainPtr = 0;
ID3D11RenderTargetView* Renderer::theRenderTargetViewPtr = 0;
ID3D11Texture2D* Renderer::theBackBufferPtr = 0;
ID3D11Texture2D* Renderer::theDepthStencilBufferPtr = 0;
ID3D11DepthStencilView* Renderer::theDepthStencilViewPtr = 0;
ID3D11ShaderResourceView* Renderer::theDepthStencilSRVPtr = 0;
ID3D11RasterizerState* Renderer::theDefaultRasterState = 0;
ID3D11InputLayout* Renderer::theDefaultInputLayout = 0;
ID3D11SamplerState* Renderer::theSamplerState = 0;

D3D11_VIEWPORT Renderer::theScreenViewport;
UINT Renderer::resolutionWidth = 0;
UINT Renderer::resolutionHeight = 0;

RenderObject* Renderer::testOBJ = 0;
ID3D11Buffer* Renderer::constantBuffer = 0;
cb_DEFAULT* Renderer::toShader = 0;

Camera* Renderer::MainCamera = 0;
RenderList* Renderer::m_pRenderList = 0;

bool fullscreen = true;

#define SAMPLE_COUNT 8

void Renderer::Initialize(HWND hWnd, UINT resWidth, UINT resHeight)
{
	resolutionWidth = resWidth;
	resolutionHeight = resHeight;

	D3D11_CREATE_DEVICE_FLAG flag = (D3D11_CREATE_DEVICE_FLAG)0;

#if _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = resolutionWidth;
	scd.BufferDesc.Height = resolutionHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // using 32 bit color
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = SAMPLE_COUNT;
	scd.Windowed = fullscreen;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flag,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&theSwapChainPtr,
		&theDevicePtr,
		NULL,
		&theContextPtr);

	D3D11_RASTERIZER_DESC DefaultRasterDesc;
	ZeroMemory(&DefaultRasterDesc, sizeof(DefaultRasterDesc));
	DefaultRasterDesc.AntialiasedLineEnable = TRUE;
	DefaultRasterDesc.MultisampleEnable = TRUE;
	DefaultRasterDesc.CullMode = D3D11_CULL_BACK;
	DefaultRasterDesc.FillMode = D3D11_FILL_SOLID;
	theDevicePtr->CreateRasterizerState(&DefaultRasterDesc, &theDefaultRasterState);

	theContextPtr->RSSetState(theDefaultRasterState);

	Shaders::GetInstance()->CreateShaders();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	int elements = sizeof(layout) / sizeof(layout[0]);
	theDevicePtr->CreateInputLayout(layout,
		elements,
		Generic_VS,
		sizeof(Generic_VS),
		&theDefaultInputLayout);
	theContextPtr->IASetInputLayout(theDefaultInputLayout);

	toShader = new cb_DEFAULT();
	MainCamera = new Camera();

	MainCamera->BuildProjection(resWidth, resHeight);

	Float4x4 identity = IDENTITY;
	Float4x4 mvp;

	MainCamera->GetMVP(identity, mvp);
	toShader->gMVP = mvp;
	toShader->gWorld = identity;

	D3D11_BUFFER_DESC cBufferData;
	ZeroMemory(&cBufferData, sizeof(cBufferData));
	cBufferData.ByteWidth = sizeof(cb_DEFAULT);
	cBufferData.Usage = D3D11_USAGE_DYNAMIC;
	cBufferData.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferData.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA cData;
	ZeroMemory(&cData, sizeof(cData));
	cData.pSysMem = toShader;
	theDevicePtr->CreateBuffer(&cBufferData, &cData, &constantBuffer);

	D3D11_SAMPLER_DESC sData;
	ZeroMemory(&sData, sizeof(sData));
	sData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sData.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sData.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sData.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sData.MaxLOD = D3D11_FLOAT32_MAX;
	sData.MinLOD = -D3D11_FLOAT32_MAX;

	theDevicePtr->CreateSamplerState(&sData, &theSamplerState);

	m_pRenderList = new RenderList();

	testOBJ = new RenderObject("Car");
	m_pRenderList->AddRenderObject(testOBJ);
}

void Renderer::SetResolution(UINT _width, UINT _height)
{
	ReleaseCOM(theRenderTargetViewPtr);
	ReleaseCOM(theDepthStencilBufferPtr);
	ReleaseCOM(theDepthStencilViewPtr);
	ReleaseCOM(theBackBufferPtr);

	resolutionWidth = _width;
	resolutionHeight = _height;
	ResizeBuffers();

	MainCamera->BuildProjection(_width, _height);
}

void Renderer::ResizeBuffers()
{
	if (theRenderTargetViewPtr != nullptr)
	{
		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theBackBufferPtr);
	}

	theSwapChainPtr->GetBuffer(0, _uuidof(ID3D11Texture2D), (LPVOID*)&theBackBufferPtr);

	theDevicePtr->CreateRenderTargetView(theBackBufferPtr, nullptr, &theRenderTargetViewPtr);

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = resolutionWidth;
	descDepth.Height = resolutionHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	descDepth.SampleDesc.Count = SAMPLE_COUNT;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	theDevicePtr->CreateTexture2D(&descDepth, NULL, &theDepthStencilBufferPtr);


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	theDevicePtr->CreateDepthStencilView(theDepthStencilBufferPtr, &descDSV, &theDepthStencilViewPtr);
	theContextPtr->OMSetRenderTargets(1, &theRenderTargetViewPtr, theDepthStencilViewPtr);

	ZeroMemory(&theScreenViewport, sizeof(D3D11_VIEWPORT));

	theScreenViewport.TopLeftX = 0;
	theScreenViewport.TopLeftY = 0;
	theScreenViewport.Width = (float)resolutionWidth;
	theScreenViewport.Height = (float)resolutionHeight;
	theScreenViewport.MinDepth = 0;
	theScreenViewport.MaxDepth = 1;

	theContextPtr->RSSetViewports(1, &theScreenViewport);
	theContextPtr->VSSetConstantBuffers(0, 1, &constantBuffer);
}

void Renderer::Render()
{
	MainCamera->Update(resolutionWidth, resolutionHeight, 0.01f);

	float clearColor[4] = { 1.0f, 0.41f, 0.71f, 1.0f };
	theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, clearColor);
	theContextPtr->ClearDepthStencilView(theDepthStencilViewPtr, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pRenderList->Render();

	theSwapChainPtr->Present(0,0);
}


void Renderer::Shutdown()
{
	theSwapChainPtr->SetFullscreenState(false, 0);
	
	// release the d3d object and device
	BufferManager::GetInstance()->unload();
	Shaders::GetInstance()->unload();
	ReleaseCOM(theSamplerState);
	ReleaseCOM(constantBuffer);
	ReleaseCOM(theDefaultInputLayout);
	ReleaseCOM(theDefaultRasterState);
	ReleaseCOM(theDepthStencilSRVPtr);
	ReleaseCOM(theDepthStencilViewPtr);
	ReleaseCOM(theDepthStencilBufferPtr);
	ReleaseCOM(theBackBufferPtr);
	ReleaseCOM(theRenderTargetViewPtr);

	ReleaseCOM(theSwapChainPtr);
	ReleaseCOM(theContextPtr);
	ReleaseCOM(theDevicePtr);
}