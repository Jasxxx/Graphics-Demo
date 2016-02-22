#include "Shaders.h"
#include "Generic_PS.csh"
#include "HullShader.csh"
#include "DomainShader.csh"
#include "TerrainTess_VS.csh"
#include "Sky_VS.csh"
#include "Sky_PS.csh"

Shaders* Shaders::m_pInstance = 0;

Shaders * Shaders::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new Shaders();
	}

	return m_pInstance;
}

void Shaders::CreateShaders()
{
	Renderer::theDevicePtr->CreateVertexShader(Generic_VS,
		sizeof(Generic_VS),
		NULL,
		&m_pGeneric_VS);
	Renderer::theDevicePtr->CreatePixelShader(Generic_PS,
		sizeof(Generic_PS),
		NULL,
		&m_pGeneric_PS);
	Renderer::theDevicePtr->CreateHullShader(HullShader,
		sizeof(HullShader),
		NULL,
		&m_pHullShader);
	Renderer::theDevicePtr->CreateDomainShader(DomainShader,
		sizeof(DomainShader),
		NULL,
		&m_pDomainShader);
	Renderer::theDevicePtr->CreateVertexShader(TerrainTess_VS,
		sizeof(TerrainTess_VS),
		NULL,
		&m_pTerrainTess_VS);
	Renderer::theDevicePtr->CreateVertexShader(Sky_VS,
		sizeof(Sky_VS),
		NULL,
		&m_pSky_VS);
	Renderer::theDevicePtr->CreatePixelShader(Sky_PS,
		sizeof(Sky_PS),
		NULL,
		&m_pSky_PS);
}

void Shaders::unload()
{
	ReleaseCOM(m_pSky_PS);
	ReleaseCOM(m_pSky_VS);
	ReleaseCOM(m_pTerrainTess_VS);
	ReleaseCOM(m_pDomainShader);
	ReleaseCOM(m_pHullShader);
	ReleaseCOM(m_pGeneric_PS);
	ReleaseCOM(m_pGeneric_VS);
}
