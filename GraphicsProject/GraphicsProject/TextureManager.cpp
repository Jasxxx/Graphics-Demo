#include "TextureManager.h"
#include "Renderer.h"
#include "DDSTextureLoader.h"

TextureManager* TextureManager::m_pInstance = 0;

TextureManager * TextureManager::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new TextureManager();
	}

	return m_pInstance;
}

ID3D11ShaderResourceView* TextureManager::GetTexture(std::string name)
{
	//const wchar_t temp = name.c_str();
	wstring temp;
	for (unsigned int i = 0; i < name.length(); i++)
	{
		temp += wchar_t(name[i]);
	}
	
	if (textureMap[name])
	{
		return textureMap[name];
	}

	temp += L".dds";
	CreateDDSTextureFromFile(Renderer::theDevicePtr, temp.c_str(), NULL, &textureMap[name]);
	return textureMap[name];
}
