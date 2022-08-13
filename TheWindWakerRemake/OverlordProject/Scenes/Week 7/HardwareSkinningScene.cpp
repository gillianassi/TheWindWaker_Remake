#include "stdafx.h"
#include "HardwareSkinningScene.h"

#include "Materials/DiffuseMaterial_Skinned.h"

HardwareSkinningScene::~HardwareSkinningScene()
{
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		delete[] m_ClipNames[i];
	}

	delete[] m_ClipNames;
}

void HardwareSkinningScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	const auto pSwordMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pSwordMaterial->SetDiffuseTexture(L"Textures/Link/sheath.png");
	const auto pEyebrowRMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pEyebrowRMaterial->SetDiffuseTexture(L"Textures/Link/eyebrow1.png");
	const auto pEyebrowLMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pEyebrowLMaterial->SetDiffuseTexture(L"Textures/Link/eyebrow1.png");
	const auto pMouthMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pMouthMaterial->SetDiffuseTexture(L"Textures/Link/mouth1.png");
	const auto pPupilRMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPupilRMaterial->SetDiffuseTexture(L"Textures/Link/pupil.png");
	const auto pEyeRMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pEyeRMaterial->SetDiffuseTexture(L"Textures/Link/eye1.png");
	const auto pPupilLMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPupilLMaterial->SetDiffuseTexture(L"Textures/Link/pupil.png");
	const auto pEyeLMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pEyeLMaterial->SetDiffuseTexture(L"Textures/Link/eye1.png");
	const auto pBodyMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pBodyMaterial->SetDiffuseTexture(L"Textures/Link/body.png");

	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Link.ovm"));
	pModel->SetMaterial(pSwordMaterial, 0);
	pModel->SetMaterial(pEyebrowRMaterial, 1);
	pModel->SetMaterial(pEyebrowLMaterial, 2);
	pModel->SetMaterial(pMouthMaterial, 3);
	pModel->SetMaterial(pPupilRMaterial, 4);
	pModel->SetMaterial(pEyeRMaterial, 5);
	pModel->SetMaterial(pPupilLMaterial, 6);
	pModel->SetMaterial(pEyeLMaterial, 7);
	pModel->SetMaterial(pBodyMaterial, 8);

	pObject->GetTransform()->Scale(0.15f);

	pAnimator = pModel->GetAnimator();
	pAnimator->SetAnimation(m_AnimationClipId);
	pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = pAnimator->GetClipCount();
	m_ClipNames = new char* [m_ClipCount];
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		auto clipName = StringUtil::utf8_encode(pAnimator->GetClip(static_cast<int>(i)).name);
		const auto clipSize = clipName.size();
		m_ClipNames[i] = new char[clipSize + 1];
		strncpy_s(m_ClipNames[i], clipSize + 1, clipName.c_str(), clipSize);
	}
}

void HardwareSkinningScene::OnGUI()
{
	if (ImGui::Button(pAnimator->AnimationComplete() ? "PAUSE" : "PLAY"))
	{
		if (pAnimator->AnimationComplete())pAnimator->Pause();
		else pAnimator->Play();
	}

	if (ImGui::Button("RESET"))
	{
		pAnimator->Reset();
	}

	ImGui::Dummy({ 0,5 });

	bool reversed = pAnimator->IsReversed();
	if (ImGui::Checkbox("Play Reversed", &reversed))
	{
		pAnimator->SetPlayReversed(reversed);
	}

	if (ImGui::ListBox("Animation Clip", &m_AnimationClipId, m_ClipNames, static_cast<int>(m_ClipCount)))
	{
		pAnimator->SetAnimation(m_AnimationClipId);
	}

	if (ImGui::SliderFloat("Animation Speed", &m_AnimationSpeed, 0.f, 4.f))
	{
		pAnimator->SetAnimationSpeed(m_AnimationSpeed);
	}
}