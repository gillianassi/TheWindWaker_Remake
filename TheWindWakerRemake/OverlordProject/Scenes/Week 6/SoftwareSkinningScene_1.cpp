#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"


#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	// Create a bone material
	ColorMaterial* pBoneMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();


	// Create the root of the bone hierarchy
	GameObject* pRoot = AddChild(new GameObject());
	// initialize the first bone
	m_pBone0 = new BoneObject(pBoneMaterial, 15.f);
	pRoot->AddChild(m_pBone0);
	m_pBone1 = new BoneObject(pBoneMaterial, 15.f);
	m_pBone0->AddChildBone(m_pBone1);



}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		if (m_RotationSign == 1)
		{
			if (m_Bone0Rotation.z >= 45)
			{
				m_RotationSign = -1;
			}
		}
		else
		{
			if (m_Bone0Rotation.z <= -45)
			{
				m_RotationSign = 1;
			}
		}

		m_Bone0Rotation.z += m_RotationSign * GetSceneContext().pGameTime->GetElapsed() * 45;
		m_Bone1Rotation.z = m_Bone0Rotation.z * -2.f;

	}
		m_pBone0->GetTransform()->Rotate(m_Bone0Rotation);
		m_pBone1->GetTransform()->Rotate(m_Bone1Rotation);
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::DragFloat3("Bone0 Rotation (degrees):", &m_Bone0Rotation.x, 0.1f, 0.f, 0.f, "%.1f");
	ImGui::DragFloat3("Bone1 Rotation (degrees):", &m_Bone1Rotation.x, 0.1f, 0.f, 0.f, "%.1f");

	ImGui::Checkbox("AutoRotate", &m_AutoRotate);
}
