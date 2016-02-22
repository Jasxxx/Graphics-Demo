#pragma once
#include "GraphicsStructs.h"
#include "Renderer.h"


using namespace DirectX;

enum CONSTANT_TYPE { DEFAULT, DIR_LIGHT, POINT_LIGHT, SPOT_LIGHT, ANIMATED };

struct cb_DEFAULT
{
	Float4x4 gMVP;
	Float4x4 gWorld;
};

struct cb_DIRECTIONAL_LIGHT
{
	XMFLOAT3 dir;
	float specularPower;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 CameraPosition;
	float padding;
	XMFLOAT3 position;
	float range;
	XMFLOAT3 attenuation;
	float pad2;
};

struct cb_POINT_LIGHT
{
	XMFLOAT3 postion;
	float radius;
	XMFLOAT4 color;
	XMFLOAT3 att;
	float pad2;
};

struct cb_SPOT_LIGHT
{
	XMFLOAT3 position;
	float cone;
	XMFLOAT4 color;
	XMFLOAT3 direction;
	float pad;
};

static void CreateConstantBuffer(ID3D11Buffer* cBuffer, D3D11_USAGE usage, D3D11_BIND_FLAG flag, D3D11_CPU_ACCESS_FLAG access, CONSTANT_TYPE type, void* data, ID3D11Device* device)
{
	cb_DEFAULT* temp = (cb_DEFAULT*)data;

	D3D11_BUFFER_DESC cBufferData;
	ZeroMemory(&cBufferData, sizeof(cBufferData));
	cBufferData.ByteWidth = sizeof(&data);
	cBufferData.Usage = usage;
	cBufferData.BindFlags = flag;
	cBufferData.CPUAccessFlags = access;

	D3D11_SUBRESOURCE_DATA cData;
	ZeroMemory(&cData, sizeof(cData));
	cData.pSysMem = data;
	device->CreateBuffer(&cBufferData, &cData, &cBuffer);
}
