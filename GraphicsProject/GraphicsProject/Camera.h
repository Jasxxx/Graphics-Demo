#pragma once
#include "GraphicsStructs.h"

class Camera
{
public:
	Camera();
	~Camera();

	Float4x4 m_mTransform;
	Float4x4 m_mView;
	Float4x4 m_mInvView;
	Float4x4 m_mProjection;
	POINT m_lpCurrentPosition;
	POINT m_lpPreviousPosition;
	bool buttonDown;
	void Update(int width, int height, float delta);
	void BuildProjection(int width, int height);
	void GetMVP(Float4x4 world, Float4x4& mvp);
	void ConstructMV();

};