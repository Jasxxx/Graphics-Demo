#include "TerrainRenderObject.h"
#include "TextureManager.h"


TerrainRenderObject::TerrainRenderObject(std::string name, D3D11_PRIMITIVE_TOPOLOGY primT) : RenderObject(name, primT)
{
	m_vPosition = Float3(0, 0, 0);
	m_pTexture = TextureManager::GetInstance()->GetTexture("Mount_Texture");
	m_pHeightMap = TextureManager::GetInstance()->GetTexture("Mount_HeightMap");
}


TerrainRenderObject::~TerrainRenderObject()
{
}
