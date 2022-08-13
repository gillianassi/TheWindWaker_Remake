#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		float elapsedTime = sceneContext.pGameTime->GetElapsed();
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = elapsedTime * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, m_CurrentClip.duration);
		


		AnimationKey keyA, keyB;
		m_TickCount = CalculateTick(passedTicks, true);
		CalculateAnimationKeys(m_TickCount, m_CurrentClip, keyA, keyB);

		//Clear the m_Transforms vector
		m_Transforms.clear();
		m_Transforms = HandleAnimation(m_TickCount, keyA, keyB);

		// If we're transitionning, blend inbetween the transition
		if (m_ElapsedBlendTime < m_BlendDuration)
		{
			
			//Calculate the passedTicks (see the lab document)
			float passedOldTicks = elapsedTime * m_OldClip.ticksPerSecond * m_AnimationSpeed;
			//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
			passedOldTicks = fmod(passedOldTicks, m_OldClip.duration);
			AnimationKey keyC, keyD;
			m_OldTickCount = CalculateTick(passedTicks, false);
			CalculateAnimationKeys(m_OldTickCount, m_OldClip, keyC, keyD);
			std::vector<XMFLOAT4X4> oldAnimTransforms = HandleAnimation(m_OldTickCount, keyC, keyD);

			float blendAmmount = m_ElapsedBlendTime / m_BlendDuration;


			m_ElapsedBlendTime += elapsedTime;

			

			for (size_t i = 0; i < oldAnimTransforms.size(); i++)
			{
				m_Transforms[i] = CalculateInterpolatedTransform(oldAnimTransforms[i], m_Transforms[i], blendAmmount);
			}
		}
	}
}

float ModelAnimator::CalculateTick(float passedTicks, bool isCurrentClip)
{
	float tickCount;
	AnimationClip clip;
	bool looping;
	if (isCurrentClip)
	{
		tickCount = m_TickCount;
		clip = m_CurrentClip;
		looping = m_IsLooping;
	}
	else
	{
		tickCount = m_OldTickCount;
		clip = m_OldClip;
		looping = m_IsOldLooping;
	}
	//2. 
	if (m_Reversed)
	{
		//	go in reversed direction
		tickCount -= passedTicks;
		// in reversed direction when your animation is passed the initial point -> You have the negative remainder
		// If you underflow the duration
		if (tickCount < 0)
		{
			if (!looping)
			{
				return tickCount;
			}
			// add the duration of the clip to get the current time stamp
			// same as duration - abs(underflowed time)
			tickCount += clip.duration;

		}
	}
	else // the animation is not reversed
	{
		//	simply add the elapsed ticks
		tickCount += passedTicks;
		// Note: this part could be in the else above?
		// if you overflow the duration
		if (tickCount >= clip.duration)
		{
			if (!looping)
			{
				return tickCount;
			}
			// only keep the remaining time
			tickCount -= clip.duration;
		}
	}

	return tickCount;
}


void ModelAnimator::CalculateAnimationKeys(float tickCount, const AnimationClip& clip, AnimationKey& keyA, AnimationKey& keyB)
{
	if (tickCount >= clip.duration)
	{
		tickCount = clip.duration;
		//keyA = clip.keys[clip.keys.size() -2];
		//keyB = clip.keys[clip.keys.size() -1];
		//return;
	}
	if(tickCount < 0)
	{
		tickCount = 0;
		//keyA = clip.keys[0];
		//keyB = clip.keys[1];
		//return;
	}

	//3.
	//Find the enclosing keys
	//Iterate all the keys of the clip and find the following keys:
	for (AnimationKey key : clip.keys)
	{
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		if (key.tick <= tickCount)
		{
			keyA = key;
		}
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		else
		{
			keyB = key;
			// the tick goes up, so this will be the last relevant tick
			break;
		}
	}
	if (keyB.boneTransforms.size() == 0)
	{

		keyB = clip.keys[0];
	}
	if (keyA.boneTransforms.size() == 0)
	{

		keyA = clip.keys[0];
	}
}


std::vector<XMFLOAT4X4> ModelAnimator::HandleAnimation(float tickCount, AnimationKey keyA, AnimationKey keyB)
{
	std::vector<XMFLOAT4X4> transforms{};
	//4.
	//Interpolate between keys
	//Figure out the BlendFactor (See lab document)
	float blendFactor = tickCount - keyA.tick;
	float keyDuration = keyB.tick - keyA.tick;
	blendFactor /= keyDuration;


	//FOR every boneTransform in a key (So for every bone)
	for (size_t j = 0; j < m_pMeshFilter->m_BoneCount; j++)
	{
		//	Retrieve the transform from keyA (transformA)
		const XMFLOAT4X4 transformA = keyA.boneTransforms[j];
		// 	Retrieve the transform from keyB (transformB)
		const XMFLOAT4X4 transformB = keyB.boneTransforms[j];

		//	Add the resulting matrix to the m_Transforms vector
		transforms.push_back(CalculateInterpolatedTransform(transformA, transformB, blendFactor));
	}

	return transforms;
}

XMFLOAT4X4 ModelAnimator::CalculateInterpolatedTransform(const XMFLOAT4X4& transformA, const XMFLOAT4X4& transformB, float blendFactor)
{
	//	Decompose both transforms
	XMMATRIX transformAMatrix = XMLoadFloat4x4(&transformA);
	XMMATRIX transformBMatrix = XMLoadFloat4x4(&transformB);

	XMVECTOR scaleA{};
	XMVECTOR rotQuatA{};
	XMVECTOR transA{};
	XMMatrixDecompose(&scaleA, &rotQuatA, &transA, transformAMatrix);
	XMVECTOR scaleB{};
	XMVECTOR rotQuatB{};
	XMVECTOR transB{};
	XMMatrixDecompose(&scaleB, &rotQuatB, &transB, transformBMatrix);
	//	Lerp between all the transformations (Position, Scale, Rotation)
	XMVECTOR scale = XMVectorLerp(scaleA, scaleB, blendFactor);
	XMVECTOR rotationQuat = XMQuaternionSlerp(rotQuatA, rotQuatB, blendFactor);
	XMVECTOR translation = XMVectorLerp(transA, transB, blendFactor);
	//	Compose a transformation matrix with the lerp-results
	XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMMATRIX finalTranformMatrix = XMMatrixAffineTransformation(scale, zero, rotationQuat, translation);
	DirectX::XMFLOAT4X4 finalTransform = {};
	DirectX::XMStoreFloat4x4(&finalTransform, finalTranformMatrix);
	return finalTransform;
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	auto it = std::find_if(m_pMeshFilter->m_AnimationClips.begin(), m_pMeshFilter->m_AnimationClips.end(),
		[clipName](AnimationClip clip)
		{
			return clip.name == clipName;
		});

	if (it != m_pMeshFilter->m_AnimationClips.end())
	{
		SetAnimation(*it);
	}
	else
	{
		Reset();
		Logger::LogWarning(L"Warning: the clip name \"{}\" does not exist in the currrent clip vector.", clipName.c_str());
	}
}

void ModelAnimator::SetAnimation(UINT clipNumber, bool isLooping)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	// set looping
	m_IsLooping = isLooping;
	m_Reversed = false;
	m_IsPlaying = true;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{		
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		AnimationClip animationClip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(animationClip);
	}
	else
	{
		Reset();
		Logger::LogWarning(L"Warning: Accessing a value outside vector range. The clip numpber \"{}\" is larger than the animation clips vector size", clipNumber);
	}
		
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{

	m_ElapsedBlendTime = 0.f;
	//Set m_ClipSet to true
	m_ClipSet = true;
	if (m_IsFirstAnimation)
	{
		m_OldClip = clip;
		m_IsFirstAnimation = false;
	}
	else
	{
		m_OldClip = m_CurrentClip;
	}
	m_CurrentClip = clip;
	m_IsOldLooping = m_IsLooping;
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	if (pause)
	{
		m_IsPlaying = false;
	}
	m_OldTickCount = m_TickCount;
	m_TickCount = 0;
	//m_AnimationSpeed = 1.0f;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		std::vector<XMFLOAT4X4> boneTransforms = m_CurrentClip.keys[0].boneTransforms;
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(boneTransforms.begin(), boneTransforms.end());
	}
	else
	{
		// create an identity matrix
		XMFLOAT4X4 identity{};
		XMStoreFloat4x4(&identity, XMMatrixIdentity());
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		m_Transforms.assign(m_Transforms.size(), identity);
	}

}
