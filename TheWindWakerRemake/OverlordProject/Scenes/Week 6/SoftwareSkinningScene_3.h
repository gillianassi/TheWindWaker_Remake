#pragma once

class BoneObject;
class SoftwareSkinningScene_3 :public GameScene
{
public:
	SoftwareSkinningScene_3() :
		GameScene(L"SoftwareSkinningScene_3"),
		m_pBone0{},
		m_pBone1{},
		m_pMeshDrawer{},
		m_SkinnedVertices{},
		m_Bone0Rotation{},
		m_Bone1Rotation{},
		m_RotationSign{ 1 },
		m_MaxAngle{ 45.f },
		m_Length{ 15.f },
		m_AutoRotate{ false }
	{}

	~SoftwareSkinningScene_3() override = default;
	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float blendWeight0 = 0.5f) :
			transformedVertex{ position, normal, color },
			originalVertex{ position, normal, color },
			blendWeight0{ blendWeight0 },
			blendWeight1{ 1.f - blendWeight0 }
		{}

		// Note: with only two bones, blendweight 1 will always be 1- blendweight0
		float blendWeight0{};
		float blendWeight1{};
		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};


	};
	void InitializeVertices(float length);

	BoneObject* m_pBone0;
	BoneObject* m_pBone1;

	MeshDrawComponent* m_pMeshDrawer;
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices;

	XMFLOAT3 m_Bone0Rotation;
	XMFLOAT3 m_Bone1Rotation;
	int m_RotationSign;
	const float m_MaxAngle;
	const float m_Length;

	bool m_AutoRotate;
};

