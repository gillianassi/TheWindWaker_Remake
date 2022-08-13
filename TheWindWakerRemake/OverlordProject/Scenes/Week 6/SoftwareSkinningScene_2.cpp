#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"


#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"

void SoftwareSkinningScene_2::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	// Create a bone material
	ColorMaterial* pBoneMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();


	// Create the root of the bone hierarchy
	GameObject* pRoot = AddChild(new GameObject());
	// initialize the first bone
	m_pBone0 = new BoneObject(pBoneMaterial, m_Length);
	pRoot->AddChild(m_pBone0);
	m_pBone1 = new BoneObject(pBoneMaterial, m_Length);
	m_pBone0->AddChildBone(m_pBone1);



	// Calculate the Binde pose of the highest parent AFTER EVERY CHILD IS ASSIGNED
	// Note: Do not calculate the bindpose for the child, as the parent will take care of this
	// WARNING: If you are going to move the bone, you need to recalculate the bind pose! not onlin during initialisation.
	m_pBone0->CalculateBindPose();


	// Add MeshdrawComponent
	GameObject* pBoxDrawer = AddChild(new GameObject());
	m_pMeshDrawer = pBoxDrawer->AddComponent(new MeshDrawComponent(24U, true));


	// initialize the vertex software skinned
	InitializeVertices(m_Length);

}

void SoftwareSkinningScene_2::Update()
{

	// Handle Rotation
	if (m_AutoRotate)
	{
		if (m_RotationSign == 1)
		{
			if (m_Bone0Rotation.z >= m_MaxAngle)
			{
				m_RotationSign = -1;
			}
		}
		else
		{
			if (m_Bone0Rotation.z <= -m_MaxAngle)
			{
				m_RotationSign = 1;
			}
		}

		m_Bone0Rotation.z += m_RotationSign * GetSceneContext().pGameTime->GetElapsed() * 45;
		m_Bone1Rotation.z = m_Bone0Rotation.z * -2.f;

	}
	m_pBone0->GetTransform()->Rotate(m_Bone0Rotation);
	m_pBone1->GetTransform()->Rotate(m_Bone1Rotation);

	// Visualise the boxes
	m_pMeshDrawer->RemoveTriangles();
	
	// bone 0
	XMMATRIX bindPose = XMLoadFloat4x4(&m_pBone0->GetBindPose());
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld());
	XMMATRIX boneTransform0 = XMMatrixMultiply(bindPose, worldMatrix);
	// bone 1
	bindPose = XMLoadFloat4x4(&m_pBone1->GetBindPose());
	worldMatrix = XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld());
	XMMATRIX boneTransform1 = XMMatrixMultiply(bindPose, worldMatrix);

	// Transform the position of each original vertex between the bind pose of the bone and the worldmatrix of the bone.
	XMVECTOR originalVertex{};
	XMVECTOR transformedVertex{};

	for (size_t i = 0; i < m_SkinnedVertices.size(); ++i)
	{

		originalVertex = XMLoadFloat3(&m_SkinnedVertices[i].originalVertex.Position);
		// first box
		if (i < 24)
		{
			transformedVertex =  XMVector3TransformCoord(originalVertex, boneTransform0);

		}
		// second box
		else
		{
			transformedVertex = XMVector3TransformCoord(originalVertex, boneTransform1);
		}

		XMStoreFloat3(&m_SkinnedVertices[i].transformedVertex.Position, transformedVertex);
	}


	// itterate and append each set of 4 verticies as a quad
	for (size_t i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		QuadPosNormCol quad = QuadPosNormCol(
			m_SkinnedVertices[i].transformedVertex, 
			m_SkinnedVertices[i + 1].transformedVertex, 
			m_SkinnedVertices[i + 2].transformedVertex, 
			m_SkinnedVertices[i + 3].transformedVertex);
		m_pMeshDrawer->AddQuad(quad);
	}
	// update buffer
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::OnGUI()
{

	ImGui::DragFloat3("Bone0 Rotation (degrees):", &m_Bone0Rotation.x, 0.1f, 0.f, 0.f, "%.1f");
	ImGui::DragFloat3("Bone1 Rotation (degrees):", &m_Bone1Rotation.x, 0.1f, 0.f, 0.f, "%.1f");

	ImGui::Checkbox("AutoRotate", &m_AutoRotate);
}

void SoftwareSkinningScene_2::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion
}
