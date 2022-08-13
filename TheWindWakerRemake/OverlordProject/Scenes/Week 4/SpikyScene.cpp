#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

SpikyScene::SpikyScene() :
	GameScene(L"UberMaterialScene"),
	m_pSpikyBall{ nullptr },
	m_pSpikyMaterial{ nullptr },
	m_Angle{}
{
}

void SpikyScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

	m_pSpikyMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	m_pSpikyBall = new GameObject();
	auto model = m_pSpikyBall->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"));
	model->SetMaterial(m_pSpikyMaterial);
	AddChild(m_pSpikyBall);

	m_pSpikyBall->GetTransform()->Scale(20, 20, 20);
}

void SpikyScene::Update()
{
	constexpr float velocity{ 20.f };
	m_Angle += velocity * m_SceneContext.pGameTime->GetElapsed();
	m_pSpikyBall->GetTransform()->Rotate(0, m_Angle, 0, true);
}

void SpikyScene::Draw()
{
}

void SpikyScene::OnGUI()
{
	m_pSpikyMaterial->DrawImGui();
}

void SpikyScene::SetupScene()
{
}
