#include "Animation.h"

Keyframe::Keyframe()
	: timePos(0.0f), translation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Keyframe::~Keyframe()
{
}

float BoneAnimation::GetStartTime() const
{
	return keyframes.front().timePos;
}

float BoneAnimation::GetEndTime() const
{
	return keyframes.back().timePos;
}

void BoneAnimation::Interpolate(float t, DirectX::XMFLOAT4X4& M) const
{
	using namespace DirectX;
	if (t <= keyframes.front().timePos)
	{
		XMVECTOR S = XMLoadFloat3(&keyframes.front().scale);
		XMVECTOR P = XMLoadFloat3(&keyframes.front().translation);
		XMVECTOR Q = XMLoadFloat4(&keyframes.front().rotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (t >= keyframes.back().timePos)
	{
		XMVECTOR S = XMLoadFloat3(&keyframes.front().scale);
		XMVECTOR P = XMLoadFloat3(&keyframes.front().translation);
		XMVECTOR Q = XMLoadFloat4(&keyframes.front().rotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (UINT i = 0; i < keyframes.size() - 1; ++i)
		{
			if (t >= keyframes[i].timePos && t <= keyframes[i + 1].timePos)
			{
				float lerpPercent = (t - keyframes[i].timePos) / (keyframes[i + 1].timePos - keyframes[i].timePos);

				XMVECTOR s0 = XMLoadFloat3(&keyframes[i].scale);
				XMVECTOR s1 = XMLoadFloat3(&keyframes[i + 1].scale);

				XMVECTOR p0 = XMLoadFloat3(&keyframes[i].translation);
				XMVECTOR p1 = XMLoadFloat3(&keyframes[i + 1].translation);

				XMVECTOR q0 = XMLoadFloat4(&keyframes[i].rotationQuat);
				XMVECTOR q1 = XMLoadFloat4(&keyframes[i + 1].rotationQuat);

				XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
				XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
				XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));

				break;
			}
		}

	}
}
