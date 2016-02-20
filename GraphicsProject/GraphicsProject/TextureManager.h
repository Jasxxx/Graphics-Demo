#pragma once
#include <map>
#include "GraphicsStructs.h"

using namespace DirectX;
using namespace std;

class TextureManager
{
public:
	std::map<std::string, ID3D11ShaderResourceView*> textureMap;
	static TextureManager* GetInstance();
	ID3D11ShaderResourceView* GetTexture(std::string name);
private:
	static TextureManager* m_pInstance;
};

