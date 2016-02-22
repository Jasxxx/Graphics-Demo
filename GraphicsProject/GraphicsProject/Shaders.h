#pragma once
#include "Renderer.h"
#include "Generic_VS.csh"

class Shaders
{
private:
	static Shaders* m_pInstance;
public:
	ID3D11VertexShader* m_pGeneric_VS;
	ID3D11PixelShader* m_pGeneric_PS;

	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;
	ID3D11VertexShader* m_pTerrainTess_VS;

	ID3D11VertexShader* m_pSky_VS;
	ID3D11PixelShader* m_pSky_PS;

	static Shaders* GetInstance();
	void CreateShaders();
	void unload();
};