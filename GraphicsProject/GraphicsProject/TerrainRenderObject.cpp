#include "TerrainRenderObject.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "ConstantBuffers.h"
#include "Shaders.h"

TerrainRenderObject::TerrainRenderObject(std::string name, D3D11_PRIMITIVE_TOPOLOGY primT) : RenderObject(name, primT)
{
	m_vPosition = Float3(0, 0, 0);
	m_pTexture = TextureManager::GetInstance()->GetTexture("Mount_Texture");
	m_pHeightMap = TextureManager::GetInstance()->GetTexture("Mount_HeightMap");
	/*	CreateConstantBuffer(
			pHullCB,
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			CONSTANT_TYPE::DEFAULT,
			&m_sHullCB,
			Renderer::theDevicePtr);
			*/
	D3D11_BUFFER_DESC cBufferData;
	ZeroMemory(&cBufferData, sizeof(cBufferData));
	cBufferData.ByteWidth = sizeof(m_sHullCB);
	cBufferData.Usage = D3D11_USAGE_DYNAMIC;
	cBufferData.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferData.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA cData;
	ZeroMemory(&cData, sizeof(cData));
	cData.pSysMem = &m_sHullCB;
	Renderer::theDevicePtr->CreateBuffer(&cBufferData, &cData, &pHullCB);

	m_pHullShader = Shaders::GetInstance()->m_pHullShader;
	m_pDomainShader = Shaders::GetInstance()->m_pDomainShader;
}


TerrainRenderObject::~TerrainRenderObject()
{

}

void TerrainRenderObject::Render()
{
	ID3D11DeviceContext* context = Renderer::theContextPtr;

	context->HSSetShader(m_pHullShader, NULL, 0);
	context->DSSetShader(m_pDomainShader, NULL, 0);
	context->DSSetShaderResources(0, 1, &m_pHeightMap);

	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(data));
	context->Map(pHullCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	m_sHullCB.camPos = Renderer::MainCamera->m_vPosition;
	m_sHullCB.pad = 0;

	memcpy(data.pData, &m_sHullCB, sizeof(cb_DEFAULT));
	context->Unmap(pHullCB, 0);

	context->DSSetConstantBuffers(0, 1, &Renderer::constantBuffer);
	context->DSSetSamplers(0, 1, &Renderer::theSamplerState);
	context->HSSetConstantBuffers(0, 1, &pHullCB);

	RenderObject::Render();

	context->HSSetShader(NULL, NULL, 0);
	context->DSSetShader(NULL, NULL, 0);
}