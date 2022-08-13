#pragma once

class BoneObject;
class SoftwareSkinningScene_2 : public GameScene
{

public:
	SoftwareSkinningScene_2() :
		GameScene(L"SoftwareSkinningScene_2"),
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

	~SoftwareSkinningScene_2() override = default;
	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color) :
			transformedVertex{position, normal, color},
			originalVertex{ position, normal, color }{}

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

