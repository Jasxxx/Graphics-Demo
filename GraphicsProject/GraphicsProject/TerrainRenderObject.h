#pragma once
#include "RenderObject.h"
class TerrainRenderObject :	public RenderObject
{
private:
	ID3D11DomainShader* m_pDomainShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11ShaderResourceView* m_pHeightMap;
	

public:
	TerrainRenderObject(std::string name, D3D11_PRIMITIVE_TOPOLOGY primT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~TerrainRenderObject();
};

