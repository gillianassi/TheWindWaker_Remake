#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/SpherePrefab.h"
ComponentTestScene::ComponentTestScene():
	GameScene{L"ComponentTestScene"},
	m_pBlueSphere{nullptr},
	m_pRedSphere{nullptr},
	m_pYellowSphere{nullptr}
{
}

void ComponentTestScene::Initialize()
{

	m_SceneContext.settings.enableOnGUI = true;
	auto pDefaultMaterial = PxGetPhysics().createMaterial(1.f, 1.f, 1.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	// Blue Sphere
	m_pBlueSphere = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{Colors::Blue}));
	auto pSphereActor = m_pBlueSphere->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	pSphereActor->SetCollisionGroup(CollisionGroup::Group1);
	//// Do the following if you want to make a trigger
	// 
	// UINT colliderID = pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	// auto colliderInfo = pRigidBody->GetCollider(colliderID);
	// 
	//// Collider info can only be queried after the component is intitalized (Because it is both part of the scene AND gameobject)
	// colliderInfo = pRigidbody->GetCollider(colliderID)
	// colliderInfo.SetTrigger(true);
	// 
	//// Attach Trigger Callback to owning Gameobject
	// pSphereObject->SetOnTriggerCallBack(
	// [=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
	// {
	//
	// });

	
	// Red Sphere
	m_pRedSphere = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Red }));
	pSphereActor = m_pRedSphere->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);


	// Yellow Sphere
	m_pYellowSphere = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Yellow }));
	pSphereActor = m_pYellowSphere->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	pSphereActor->SetCollisionGroup(CollisionGroup::Group2);


	// input actions
	InputAction actionReset{ int(InputIDs::Reset), InputState::released,'R', -1 };
	GetSceneContext().pInput->AddInputAction(actionReset);

	SetupScene();
}

void ComponentTestScene::Update()
{
	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::Reset)))
	{
		SetupScene();
	}
}

void ComponentTestScene::OnSceneActivated()
{
	SetupScene();
}

void ComponentTestScene::OnGUI()
{
}

void ComponentTestScene::SetupScene()
{
	m_pBlueSphere->GetTransform()->Translate(0.f, 10.f, 0.f);
	m_pBlueSphere->GetTransform()->Rotate(0.f, 0.f, 0.f);
	m_pRedSphere->GetTransform()->Translate(0.f, 30.f, 0.f);
	m_pRedSphere->GetTransform()->Rotate(0.f, 0.f, 0.f);
	m_pYellowSphere->GetTransform()->Translate(0.f, 20.f, 0.f);
	m_pYellowSphere->GetTransform()->Rotate(0.f, 0.f, 0.f);

}
