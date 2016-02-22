#pragma once
#include "RenderObject.h"
class SkySphere :
	public RenderObject
{
	int m_nFaces;
	int m_nVertCount;
	ID3D11RasterizerState* m_pRasterState;
	ID3D11DepthStencilState* m_pDepthState;
	ID3D11SamplerState* m_pSamplerState;

public:
	SkySphere(std::string name, D3D11_PRIMITIVE_TOPOLOGY primT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~SkySphere();

	void CreateSphere(int LatLines, int LongLines);
	void Render();
};

