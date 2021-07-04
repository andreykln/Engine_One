#include "CubeMapCamera.h"


CubeMapCamera::CubeMapCamera()
	: mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f * DirectX::XM_PI, 1.0f, 1.0f, 1000.0f);
}

CubeMapCamera::~CubeMapCamera()
{
}

XMVECTOR CubeMapCamera::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 CubeMapCamera::GetPosition()const
{
	return mPosition;
}

void CubeMapCamera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void CubeMapCamera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
}

XMVECTOR CubeMapCamera::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 CubeMapCamera::GetRight()const
{
	return mRight;
}

XMVECTOR CubeMapCamera::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 CubeMapCamera::GetUp()const
{
	return mUp;
}

XMVECTOR CubeMapCamera::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 CubeMapCamera::GetLook()const
{
	return mLook;
}

float CubeMapCamera::GetNearZ()const
{
	return mNearZ;
}

float CubeMapCamera::GetFarZ()const
{
	return mFarZ;
}

float CubeMapCamera::GetAspect()const
{
	return mAspect;
}

float CubeMapCamera::GetFovY()const
{
	return mFovY;
}

float CubeMapCamera::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float CubeMapCamera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float CubeMapCamera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float CubeMapCamera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float CubeMapCamera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void CubeMapCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

void CubeMapCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void CubeMapCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

XMMATRIX CubeMapCamera::View()const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX CubeMapCamera::Proj()const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX CubeMapCamera::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

void CubeMapCamera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void CubeMapCamera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

void CubeMapCamera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void CubeMapCamera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void CubeMapCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U)); 
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}
