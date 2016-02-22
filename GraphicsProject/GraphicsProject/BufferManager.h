#pragma once
#include <map>
#include "GraphicsStructs.h"

using namespace DirectX;
using namespace std;

class FBXLoader;

class BufferManager
{
	struct BUFFERINFO
	{
		unsigned int bufferSize;
		unsigned int indexCount;
		ID3D11Buffer* iBuffer;
		ID3D11Buffer* buffer;
	};

public:
	std::map<std::string, BUFFERINFO*> vertexMap;
	static BufferManager* GetInstance();
	void unload();
	enum GBuffer_Type{DIFFUSE,NORMAL,SPECULAR,BUFFER_MAX};
	void CreateVertexBuffer(std::string name);
	void CreateEmptyVertexBuffer(std::string name);

private:
	static BufferManager* m_pInstance;
	//ID3D11Texture2D* GBuffers[BUFFER_MAX];
	FBXLoader* m_pFBX;
};