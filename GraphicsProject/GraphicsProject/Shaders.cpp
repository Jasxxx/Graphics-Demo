#include "Shaders.h"

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
}

void Shaders::unload()
{
	ReleaseCOM(m_pGeneric_PS);
	ReleaseCOM(m_pGeneric_VS);
}
