#include "stdafx.h"

#include "Rupee.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/Link.h"
#include "Managers/GameSoundManager.h"

Rupee::Rupee(Link* pLink):
	m_pLink{pLink}
{}

void Rupee::Initialize(const SceneContext& sceneContext)
{
	auto soundManager = GameSoundManager::Get();
	soundManager->AddSound(GameSoundManager::Sound::GreenRupee, "Resources/Sound/SFX/WW_Get_Rupee.wav");
	soundManager->AddSound(GameSoundManager::Sound::BlueRupee, "Resources/Sound/SFX/WW_Get_Rupee_Blue.wav");
	soundManager->AddSound(GameSoundManager::Sound::YellowRupee, "Resources/Sound/SFX/WW_Get_Rupee_Yellow.wav");



	m_Rotation = float((rand() % (180 + 1)));
	m_direction = ((rand() % 2) - 1) ? 1 : - 1;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 1.f);
	const auto pMetrial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMetrial->UseTransparency(true);
	pMetrial->SetColor(m_RupeeColor);
	pMetrial->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	m_pMesh = AddChild(new GameObject());
	const auto pRupeeModel = m_pMesh->AddComponent(new ModelComponent(L"Meshes/Rupee.ovm", false));
	pRupeeModel->SetMaterial(pMetrial);
	m_pMesh->GetTransform()->Rotate(90, 0, 0, true);
	m_pMesh->GetTransform()->Scale(0.05f, 0.05f, 0.05f);

	m_pMesh->AddTagg(Tag::Collectible);

	m_pTriggerBox = AddChild(new GameObject());
	m_pTriggerBox->AddTagg(Tag::Collectible);
	auto pTriggerActor = m_pTriggerBox->AddComponent(new RigidBodyComponent(true));

	pTriggerActor->AddCollider(PxBoxGeometry(.1f, .1f, .1f), *pDefaultMaterial, true);
	
	m_pTriggerBox->SetOnTriggerCallBack(
		[&](GameObject*, GameObject* pOther, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER && pOther == m_pLink)
			{
				m_isCollected = true;
				m_pLink->CollectRupees(m_RupeeValue);
			}

		}
	);

}

void Rupee::Update(const SceneContext& sceneContext)
{
	// otherwhise it will stay in the center
	m_pTriggerBox->GetTransform()->Translate(GetTransform()->GetPosition());
	
	m_Rotation += sceneContext.pGameTime->GetElapsed() * 100.f * float(m_direction);
	m_pMesh->GetTransform()->Rotate(90, 0, m_Rotation, true);
	if (m_isCollected)
	{
		if (m_RupeeValue == 1)
		{
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::GreenRupee);
		}
		else if (m_RupeeValue == 5)
		{
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::BlueRupee);
		}
		else
		{
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::YellowRupee);
		}

		GetParent()->RemoveChild(this, true);
	}
}

void Rupee::DrawImGui()
{

}