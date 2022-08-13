#include "stdafx.h"

#include "Materials/Link/DiffuseMaterial_Toon_Skinned.h"
#include "Materials/Link/EyeMaterial.h"
#include "Materials/ColorMaterial.h"
#include "Link.h"

Link::Link(const CharacterDesc& characterDesc, PhysxProxy* pPhysxScene) :
	Character(characterDesc, pPhysxScene)
{}

void Link::Initialize(const SceneContext& sceneContext)
{
	Character::Initialize(sceneContext);


	#pragma region CreateMaterials
		const auto pSwordMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Toon_Skinned>();
		pSwordMaterial->SetDiffuseTexture(L"Textures/Link/sheath.png");
		const auto pMouthMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Toon_Skinned>();
		pMouthMaterial->SetDiffuseTexture(L"Textures/Link/mouth1.png");
		const auto pBodyMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Toon_Skinned>();
		pBodyMaterial->SetDiffuseTexture(L"Textures/Link/body.png");
		const auto pEyebrowRMaterial = MaterialManager::Get()->CreateMaterial<EyeMaterial>();
		pEyebrowRMaterial->SetDiffuseTexture(L"Textures/Link/eyebrow1.png");
		const auto pEyebrowLMaterial = MaterialManager::Get()->CreateMaterial<EyeMaterial>();
		pEyebrowLMaterial->SetDiffuseTexture(L"Textures/Link/eyebrow1.png");
		const auto pPupilRMaterial = MaterialManager::Get()->CreateMaterial<EyeMaterial>();
		pPupilRMaterial->SetDiffuseTexture(L"Textures/Link/pupil.png");
		const auto pEyeRMaterial = MaterialManager::Get()->CreateMaterial<EyeMaterial>();
		pEyeRMaterial->SetDiffuseTexture(L"Textures/Link/eye1.png");
		const auto pPupilLMaterial = MaterialManager::Get()->CreateMaterial<EyeMaterial>();
		pPupilLMaterial->SetDiffuseTexture(L"Textures/Link/pupil.png");
		const auto pEyeLMaterial = MaterialManager::Get()->CreateMaterial<EyeMaterial>();
		pEyeLMaterial->SetDiffuseTexture(L"Textures/Link/eye1.png");
	#pragma endregion

	// Give your character a model
	const auto pModel = Character::AddModel(new ModelComponent(L"Meshes/Link.ovm"));
	pModel->SetMaterial(pSwordMaterial, 0);
	pModel->SetMaterial(pMouthMaterial, 3);
	pModel->SetMaterial(pEyeRMaterial, 5, true);
	pModel->SetMaterial(pPupilRMaterial, 4, true);
	pModel->SetMaterial(pEyeLMaterial, 7, true);
	pModel->SetMaterial(pPupilLMaterial, 6, true);
	pModel->SetMaterial(pEyebrowRMaterial, 1, true);
	pModel->SetMaterial(pEyebrowLMaterial, 2, true);
	pModel->SetMaterial(pBodyMaterial, 8);




	m_pAnimator = pModel->GetAnimator();
	m_pAnimator->SetAnimation((int)m_CurrentState);
	m_pAnimator->SetAnimationSpeed(1.f);
	m_pAnimator->Play();


	// particle

	//Particle System
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,6.f,0.f };
	settings.minSize = .2f;
	settings.maxSize = .2f;
	settings.minEnergy = .5f;
	settings.maxEnergy = .8f;
	settings.minScale = 2.f;
	settings.maxScale = 3.f;
	settings.minEmitterRadius = .05f;
	settings.maxEmitterRadius = .02f;
	settings.color = { 0.81f, 0.8f, 0.7f, 0.78f };

	const auto pObject =  AddChild(new GameObject);
	m_pEmitter = pObject->AddComponent(new ParticleEmitterComponent(L"Textures/DustParticle.png", settings, 15));



}

void Link::Update(const SceneContext& sceneContext)
{
	Character::Update(sceneContext);
	m_pEmitter->GetTransform()->Translate(GetFootPosition());

	
	float particleSpeed = - 1.f;

	auto direction = GetCurrentDirection();
	// let it go slightly upwards
	m_pEmitter->GetSettings().velocity = XMFLOAT3(direction.x * particleSpeed, (direction.y - 0.015f) * particleSpeed , direction.y * particleSpeed);
	

	if (m_CurrentState == CharacterState::JUMPING)
	{

		if (m_pAnimator->AnimationComplete())
		{
			m_CurrentState = CharacterState::Falling;
		}
	}

	if (IsStateChanged())
	{
		if (m_CurrentState == CharacterState::ROLLING)
		{
			m_pEmitter->SetActive(true);
		}
		else if(m_pEmitter->IsActive())
		{
			m_pEmitter->SetActive(false);
		}
		if (m_CurrentState == CharacterState::JUMPING || m_CurrentState == CharacterState::ROLLING)
		{
			m_pAnimator->SetAnimation((UINT)m_CurrentState, false);
		}
		else if (m_CurrentState == CharacterState::CRAWL_MOVE && !isMovingForwards())
		{
			// crawl backwards
			m_pAnimator->SetAnimation((UINT)m_CurrentState);
			m_pAnimator->SetPlayReversed(true);
		}
		else
		{
			m_pAnimator->SetAnimation((UINT)m_CurrentState);
		}
	}
}

void Link::DrawImGui()
{
	Character::DrawImGui();
}