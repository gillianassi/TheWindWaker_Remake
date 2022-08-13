#include "stdafx.h"
#include "ModelTestScene.h"
#include "Materials/DiffuseMaterial.h"
#include "Managers/MaterialManager.h"

#include "Components/ModelComponent.h"
#include "Components/RigidBodyComponent.h"
ModelTestScene::ModelTestScene():
	GameScene(L"ModelTestScene"),
	m_pChair{nullptr}
{
}

void ModelTestScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	DiffuseMaterial* material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	material->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	m_pChair = new GameObject();
	auto model = m_pChair->AddComponent(new ModelComponent(L"Meshes/Chair.ovm"));
	model->SetMaterial(material);

	auto rigidBody = m_pChair->AddComponent<RigidBodyComponent>(new RigidBodyComponent());
	PxConvexMesh* pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	rigidBody->AddCollider(PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);
	AddChild(m_pChair);


	SetupScene();
}

void ModelTestScene::Update()
{
}

void ModelTestScene::Draw()
{
}

void ModelTestScene::OnGUI()
{
}

void ModelTestScene::OnSceneActivated()
{
	SetupScene();
}

void ModelTestScene::SetupScene()
{
	m_pChair->GetTransform()->Translate(0, 10, 0);
	m_pChair->GetTransform()->Rotate(0, 0, 0);
}
