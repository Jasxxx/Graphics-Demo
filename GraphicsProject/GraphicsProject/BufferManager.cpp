#include "BufferManager.h"
#include "FBXLoader.h"
#include "Renderer.h"

BufferManager* BufferManager::m_pInstance = 0;
void BufferManager::unload()
{
	map<string, BUFFERINFO*>::iterator iter;
	
	for (iter = vertexMap.begin(); iter != vertexMap.end(); iter++)
	{
		iter->second->buffer->Release();
		iter->second->iBuffer->Release();
		delete vertexMap[iter->first];
	}
}

BufferManager * BufferManager::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new BufferManager();
	}
	
	return m_pInstance;
}

void BufferManager::CreateVertexBuffer(std::string name)
{
	std::vector<VERTEX> tempVerts;
	std::string fileName = name + ".fbx";
	vector<unsigned int> uIndecies;

	m_pFBX->LoadFXB(tempVerts, fileName, uIndecies);
	tempVerts.shrink_to_fit();

	/// construct vertex buffer
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &tempVerts[0];

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(VERTEX) * tempVerts.size();

	vertexMap[name] = new BUFFERINFO();
	
	vertexMap[name]->bufferSize = tempVerts.size();
	Renderer::theDevicePtr->CreateBuffer(&desc, &data, &vertexMap[name]->buffer);

	/// construct index buffer
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &uIndecies[0];

	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = uIndecies.size() * sizeof(unsigned int);

	vertexMap[name]->indexCount = uIndecies.size();
	Renderer::theDevicePtr->CreateBuffer(&desc, &data, &vertexMap[name]->iBuffer);
}

void BufferManager::CreateEmptyVertexBuffer(std::string name)
{
	vertexMap[name] = new BUFFERINFO();
}
