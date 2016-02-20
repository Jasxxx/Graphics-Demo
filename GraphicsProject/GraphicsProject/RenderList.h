#pragma once
class RenderObject;

class RenderList
{
	struct RenderNode
	{
		RenderObject* object;
		RenderNode* next;
	};

	RenderNode* m_pHead;
	RenderNode* m_pTail;

public:
	RenderList();
	~RenderList();
	bool AddRenderObject(RenderObject* obj);
	void Render();
};

