#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")

#include "float3.h"
#include "float4x4.h"

using namespace EDMath;
using namespace DirectX;

struct VERTEX
{
	XMFLOAT4 Position;
	XMFLOAT4 Color;
	XMFLOAT2 TextureCoord;
	XMFLOAT3 Normal;

	bool VERTEX::operator==(const VERTEX& other)
	{
		if (Position.x == other.Position.x && Position.y == other.Position.y && Position.z == other.Position.z && Position.w == other.Position.w &&
			Color.x == other.Color.x && Color.y == other.Color.y && Color.z == other.Color.z && Color.w == other.Color.w &&
			TextureCoord.x == other.TextureCoord.x && TextureCoord.y == other.TextureCoord.y &&
			Normal.x == other.Normal.x && Normal.y == other.Normal.y && Normal.z == other.Normal.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct TESTVRAM
{
	Float4x4 WORLDMATRIX;
	Float4x4 VIEWMATRIX;
	Float4x4 PROJECTIONMATRIX;
};

struct HULLCB
{
	Float3 camPos;
	float pad;
};