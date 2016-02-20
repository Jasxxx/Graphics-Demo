#include "RenderObject.h"
#include "BufferManager.h"
#include "Renderer.h"
#include "Shaders.h"
#include "ConstantBuffers.h"
#include "Camera.h"
#include "DDSTextureLoader.h"
#include "TextureManager.h"

RenderObject::RenderObject(std::string name, D3D11_PRIMITIVE_TOPOLOGY primT)
{
	m_strName = name;
	BufferManager* BM = BufferManager::GetInstance();
	if (!BM->vertexMap[m_strName])
		BM->CreateVertexBuffer(m_strName);

	m_pVertexBuffer = BM->vertexMap[m_strName]->buffer;
	m_pIndexBuffer = BM->vertexMap[m_strName]->iBuffer;

	m_uPrimitiveTopology = primT;

	m_pVertexShader = Shaders::GetInstance()->m_pGeneric_VS;
	m_pPixelShader = Shaders::GetInstance()->m_pGeneric_PS;

	//CreateDDSTextureFromFile(Renderer::theDevicePtr, L"vette_color.dds", NULL, &m_pTexture);
	m_strTexture = "vette_color";
	m_pTexture = TextureManager::GetInstance()->GetTexture(m_strTexture);

	m_Position = Float3(0, 0, 0);
}

RenderObject::~RenderObject()
{

}

void RenderObject::Render()
{
	ID3D11DeviceContext* context = Renderer::theContextPtr;
	unsigned int stride = sizeof(VERTEX);
	unsigned int offset = 0;
	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(m_uPrimitiveTopology);
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->VSSetShader(m_pVertexShader, NULL, 0);
	context->PSSetShader(m_pPixelShader, NULL, 0);

	context->PSSetShaderResources(0, 1, &m_pTexture);
	context->PSSetSamplers(0, 1, &Renderer::theSamplerState);

	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(data));
	context->Map(Renderer::constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	Float4x4 temp;

	Float4x4 tempWorld = IDENTITY;
	tempWorld.translateGlobal(m_Position.x, m_Position.y, m_Position.z);
	Renderer::MainCamera->GetMVP(tempWorld, temp);

	Renderer::toShader->gMVP = temp;
	Renderer::toShader->gWorld = IDENTITY;
	/*XMStoreFloat4x4(&fIdentity, identity);
	XMStoreFloat4x4(&Renderer::toShader->gMVP, temp);
	XMStoreFloat4x4(&Renderer::toShader->gWorld, identity);*/
	memcpy(data.pData, Renderer::toShader, sizeof(cb_DEFAULT));
	context->Unmap(Renderer::constantBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &Renderer::constantBuffer);

	//Renderer::theContextPtr->Draw(BufferManager::GetInstance()->vertexMap[m_strName]->bufferSize, 0);
	Renderer::theContextPtr->DrawIndexed(BufferManager::GetInstance()->vertexMap[m_strName]->indexCount, 0, 0);
}