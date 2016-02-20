#include "RenderList.h"
#include "RenderObject.h"

RenderList::RenderList()
{
	m_pHead = nullptr;
	m_pTail = nullptr;
}


RenderList::~RenderList()
{
	while (m_pHead)
	{
		RenderNode* temp = m_pHead->next;
		delete m_pHead;
		m_pHead = temp;
	}
}

bool RenderList::AddRenderObject(RenderObject* obj)
{
	if (obj)
	{
		RenderNode* temp = new RenderNode();
		temp->object = obj;

		if (m_pHead == nullptr)
		{
			m_pHead = temp;
			m_pTail = temp;
			return true;
		}

		m_pTail->next = temp;
		m_pTail = temp;
		temp->next = nullptr;

		return true;
	}
	return false;
}

void RenderList::Render()
{
	RenderNode* temp = m_pHead;
	while (temp)
	{
		temp->object->Render();
		temp = temp->next;
	}
}
