#include "Camera.h"
#include "BufferManager.h"

Camera::Camera()
{
	m_vPosition = Float3(0, 5, -15);
	m_mView = IDENTITY;
	m_mView.WAxis = m_vPosition;
	m_fSpeed = 50.0f;
}

Camera::~Camera()
{

}

void Camera::Update(int width, int height, float delta)
{
	Float3 translation = ZERO_VECTOR;
	float deltaX = 0;
	float deltaY = 0;

	float moveFactor = m_fSpeed * delta;

	if (GetAsyncKeyState('W'))
		translation += Float3(0.0f, 0.0f, -moveFactor);
	// back
	if (GetAsyncKeyState('S'))
		translation += Float3(0.0f, 0.0f, moveFactor);
	// left
	if (GetAsyncKeyState('A'))
		translation += Float3(moveFactor, 0.0f, 0.0f);
	// right
	if (GetAsyncKeyState('D'))
		translation += Float3(-moveFactor, 0.0f, 0.0f);
	// up
	if (GetAsyncKeyState('E'))
		translation += Float3(0.0f, -moveFactor, 0.0f);
	// down
	if (GetAsyncKeyState('Q'))
		translation += Float3(0.0f, moveFactor, 0.0f);

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		GetCursorPos(&m_lpCurrentPosition);
		SetCursorPos((int)(width), (int)(height));
		GetCursorPos(&m_lpPreviousPosition);

		deltaY = (float)(m_lpPreviousPosition.y - m_lpCurrentPosition.y);
		deltaX = (float)(m_lpPreviousPosition.x - m_lpCurrentPosition.x);
	}
	// deffered context 

	Float4x4 rotationX = IDENTITY;
	Float4x4 rotationY = IDENTITY;
	rotationX.makeRotationX((float)deltaY);
	rotationY.makeRotationY((float)deltaX);

	m_mView.translateLocal(translation.negate());
	m_mView.rotateLocalX((float)-deltaY * moveFactor * 0.5f);
	m_mView.rotateLocalY((float)-deltaX * moveFactor * 0.5f);

	Float3 forward = m_mView.ZAxis;
	Float3 up;
	Float3 right;

	forward.normalize();
	CrossProduct(right, Float3(0, 1, 0), forward);
	right.normalize();
	CrossProduct(up, forward, right);
	up.normalize();

	m_mView.XAxis = right;
	m_mView.YAxis = up;
	m_mView.ZAxis = forward;

	XMFLOAT4X4 tempMat = *((XMFLOAT4X4*)&m_mView);
	XMMATRIX tempInv = XMLoadFloat4x4(&tempMat);
	tempInv = XMMatrixInverse(NULL, tempInv);
	XMStoreFloat4x4((XMFLOAT4X4*)&m_mInvView, tempInv);

	//XMMATRIX tempView = XMLoadFloat4x4(&m_mView);
	///*tempTransform = XMMatrixMultiply(rotationX, tempTransform);
	//tempTransform = XMMatrixMultiply(rotationY, tempTransform);*/
	//XMMATRIX rotation = XMMatrixRotationRollPitchYaw((float)deltaY * delta * 0.5f, (float)deltaX * delta * 0.5f, 0);
	//tempTransform = XMMatrixMultiply(rotation, tempTransform);
	//tempView = XMMatrixMultiply(tempView, tempTransform);

	//XMStoreFloat4x4(&m_mView, tempView);

	//XMFLOAT3 tempForward = XMFLOAT3(m_mView._31, m_mView._32, m_mView._33);
	//XMFLOAT3 tempUp;
	//XMFLOAT3 tempRight;
	//XMFLOAT3 tempWorldUp = XMFLOAT3(0, 1, 0);

	//XMVECTOR worldUp = XMLoadFloat3(&tempWorldUp);
	//XMVECTOR forward = XMLoadFloat3(&tempForward);
	//forward = XMVector3Normalize(forward);
	//XMVECTOR right = XMVector3Cross(worldUp, forward);
	//right = XMVector3Normalize(right);
	//XMVECTOR up = XMVector3Cross(right, forward);
	//up = XMVector3Normalize(up);

	//XMStoreFloat3(&tempUp, up);
	//XMStoreFloat3(&tempForward, forward);
	//XMStoreFloat3(&tempRight, right);

	///*m_mView._11 = tempRight.x; m_mView._12 = tempRight.y; m_mView._13 = tempRight.z;
	//m_mView._21 = tempRight.x; m_mView._22 = tempRight.y; m_mView._23 = tempRight.z;
	//m_mView._31 = tempRight.x; m_mView._32 = tempRight.y; m_mView._33 = tempRight.z;*/


	//ConstructMV();
}

void Camera::BuildProjection(int width, int height)
{
	m_mProjection.makePerspectiveLH(65 * (XM_PI / 180), (float)(width / (float)height), 0.1f, 1000.0f);
}

void Camera::GetMVP(Float4x4 world, Float4x4& mvp)
{
	mvp = world * m_mInvView;
	mvp = mvp * m_mProjection;
}

void Camera::ConstructMV()
{

}


