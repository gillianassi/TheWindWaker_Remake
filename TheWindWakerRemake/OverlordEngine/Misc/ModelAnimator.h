#pragma once
class ModelAnimator final
{
public:
	ModelAnimator(MeshFilter* pMeshFilter);
	~ModelAnimator() = default;
	ModelAnimator(const ModelAnimator& other) = delete;
	ModelAnimator(ModelAnimator&& other) noexcept = delete;
	ModelAnimator& operator=(const ModelAnimator& other) = delete;
	ModelAnimator& operator=(ModelAnimator&& other) noexcept = delete;

	void SetAnimation(const std::wstring& clipName);
	void SetAnimation(UINT clipNumber, bool isLooping = true);
	void SetAnimation(const AnimationClip& clip);
	void Update(const SceneContext& sceneContext);
	void Reset(bool pause = true);
	void Play() { m_IsPlaying = true; }
	void Pause() { m_IsPlaying = false; }
	void SetLooping(bool looping) { m_IsLooping = looping; }
	void SetPlayReversed(bool reverse) { m_Reversed = reverse; }
	void SetAnimationSpeed(float speedPercentage) { m_AnimationSpeed = speedPercentage; }


	bool AnimationComplete() const { return m_TickCount >= m_CurrentClip.duration; }
	bool IsReversed() const { return m_Reversed; }
	bool IsLooping() const { return m_IsLooping; }
	float GetAnimationSpeed() const { return m_AnimationSpeed; }
	const AnimationClip& GetClip(int clipId) { ASSERT_IF_(clipId >= m_pMeshFilter->m_AnimationClips.size())return m_pMeshFilter->m_AnimationClips[clipId]; }
	UINT GetClipCount() const { return UINT(m_pMeshFilter->m_AnimationClips.size()); }
	const std::wstring& GetClipName() const { ASSERT_IF_(!m_ClipSet) return m_CurrentClip.name; }
	const std::vector<XMFLOAT4X4>& GetBoneTransforms() const { return m_Transforms; }

private:
	float CalculateTick(float passedTicks, bool isCurrentClip);
	void CalculateAnimationKeys(float tickCount, const AnimationClip& clip, AnimationKey& keyA, AnimationKey& keyB);
	std::vector<XMFLOAT4X4> HandleAnimation(float tickCount, AnimationKey keyA, AnimationKey keyB);
	XMFLOAT4X4 CalculateInterpolatedTransform(const XMFLOAT4X4& transformA, const XMFLOAT4X4& transformB, float blendFactor);

	AnimationClip m_CurrentClip{}, m_OldClip{};
	MeshFilter* m_pMeshFilter{};
	std::vector<XMFLOAT4X4> m_Transforms{};
	bool m_IsPlaying{}, m_Reversed{}, m_ClipSet{}, m_IsLooping{ true }, m_IsFirstAnimation{ true }, m_IsOldLooping{ true };
	float m_TickCount{}, m_AnimationSpeed{1.f};
	float m_OldTickCount{};


	float m_BlendDuration{.3f}, m_ElapsedBlendTime{0.f};
};

