#include "stdafx.h"
#include "UberMaterialScene.h"

#include "Components/ModelComponent.h"
#include "Materials/UberMaterial.h"

UberMaterialScene::UberMaterialScene():
	GameScene(L"UberMaterialScene"),
	m_pUberBall{ nullptr },
	m_pUberMaterial{ nullptr },
	m_Angle{}
{
	SetupScene();
}

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

	m_pUberMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pUberBall = new GameObject();
	auto model = m_pUberBall->AddComponent(new ModelComponent(L"Meshes/Sphere.ovm"));
	model->SetMaterial(m_pUberMaterial);
	AddChild(m_pUberBall);

	m_pUberBall->GetTransform()->Scale(20,20,20);

}

void UberMaterialScene::Update()
{
	constexpr float velocity{ 50.f };
	m_Angle += velocity * m_SceneContext.pGameTime->GetElapsed();
	m_pUberBall->GetTransform()->Rotate(0, m_Angle,0, true);
}

void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	m_pUberMaterial->DrawImGui();
}

void UberMaterialScene::SetupScene()
{
}
