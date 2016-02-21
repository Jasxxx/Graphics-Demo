#pragma once
#include <string>
#include "GraphicsStructs.h"

class RenderObject
{
protected:
	std::string m_strName;
	std::string m_strTexture;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	D3D11_PRIMITIVE_TOPOLOGY m_uPrimitiveTopology;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	Float3 m_vPosition;
public:
	RenderObject() {};
	RenderObject(std::string name, D3D11_PRIMITIVE_TOPOLOGY primT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~RenderObject();
	virtual void Render();
};

