#pragma once
#include "Renderer.h"
#include "Generic_VS.csh"
#include "Generic_PS.csh"

class Shaders
{
private:
	static Shaders* m_pInstance;
public:
	ID3D11VertexShader* m_pGeneric_VS;
	ID3D11PixelShader* m_pGeneric_PS;
	static Shaders* GetInstance();
	void CreateShaders();
	void unload();
};