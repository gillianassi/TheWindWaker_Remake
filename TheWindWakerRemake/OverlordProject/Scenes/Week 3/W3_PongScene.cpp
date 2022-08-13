#include "stdafx.h"
#include "W3_PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

W3_PongScene::W3_PongScene() :
	GameScene(L"W3_PongScene"),
	m_pBall{ nullptr },
	m_pPlayer1{ nullptr },
	m_pPlayer2{ nullptr },
	m_pLeftGoal{ nullptr },
	m_pRightGoal{ nullptr },
	m_isStarted{ false },
	m_MaxDist{ 20.f },
	m_WallWidth{ 5.f },
	m_UpperBound{ m_MaxDist - m_WallWidth * 0.5f },
	m_LowerBound{ - m_MaxDist + m_WallWidth * 0.5f },
	m_PlayerLength{ 10.f },
	m_DirectionScale{ 1.f },
	m_Player1Score{ 0 },
	m_Player2Score{ 0 }
{}

void W3_PongScene::Initialize()
{
#pragma region Scene_Setup
	// Setup Scene
	m_SceneContext.settings.clearColor = XMFLOAT4{ Colors::Black };
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.enableOnGUI = true;
	auto& physx = PxGetPhysics();
	auto pDefaultMaterial = physx.createMaterial(0.f, 0.f, 1.01f);

	//Setup camera
	auto pCamera = AddChild(new FixedCamera());
	pCamera->GetTransform()->Rotate(90, 0, 0);
	pCamera->GetTransform()->Translate(0, 50, 0);
	SetActiveCamera(pCamera->GetComponent<CameraComponent>());
#pragma endregion

#pragma region Colliders

	// UPPER COLLIDER
	//-----------------
	auto pCollider = AddChild(new CubePrefab(100.f, 2.f, m_WallWidth, XMFLOAT4{ Colors::Transparent }));
	auto pColliderActor = pCollider->AddComponent(new RigidBodyComponent(true));
	pColliderActor->AddCollider(PxBoxGeometry{ 50.f, 1.f, m_WallWidth * 0.5f }, *pDefaultMaterial);
	pCollider->GetTransform()->Translate(0, 0, m_MaxDist);
	// LOWER COLLIDER
	pCollider = AddChild(new CubePrefab(100.f, 2.f, m_WallWidth, XMFLOAT4{ Colors::Transparent }));
	pColliderActor = pCollider->AddComponent(new RigidBodyComponent(true));
	pColliderActor->AddCollider(PxBoxGeometry{ 50.f, 1.f, m_WallWidth * 0.5f }, *pDefaultMaterial);
	pCollider->GetTransform()->Translate(0, 0, -m_MaxDist);
#pragma endregion

#pragma region Goals
	// P1 GOAL
	//----------
	m_pLeftGoal = AddChild(new CubePrefab(5.f, 2.f, 100.f, XMFLOAT4{ Colors::Transparent }));
	pColliderActor = m_pLeftGoal->AddComponent(new RigidBodyComponent(true));
	UINT colliderID = pColliderActor->AddCollider(PxBoxGeometry{ 2.5f, 1.f, 50.f }, *pDefaultMaterial);
	pColliderActor->GetCollider(colliderID);
	auto colliderInfo = pColliderActor->GetCollider(colliderID);

	/// Collider info can only be queried after the component is intitalized (Because it is both part of the scene AND gameobject)
	colliderInfo.SetTrigger(true);

	// Attach Trigger Callback to owning Gameobject
	m_pLeftGoal->SetOnTriggerCallBack(
	[=](GameObject*, GameObject* pOther, PxTriggerAction action)
	{
		 if (pOther == m_pBall)
		 {
			 if (action == PxTriggerAction::ENTER)
			 {
				 m_DirectionScale = 1.f;
				 ++m_Player2Score;
				 SetupScene();
			 }
		}
	});
	m_pLeftGoal->GetTransform()->Translate(-35, 0, 0);

	// P2 GOAL
	//------------
	m_pRightGoal = AddChild(new CubePrefab(5.f, 2.f, 100.f, XMFLOAT4{ Colors::Transparent }));
	pColliderActor = m_pRightGoal->AddComponent(new RigidBodyComponent(true));
	colliderID = pColliderActor->AddCollider(PxBoxGeometry{ 2.5f, 1.f, 50.f }, *pDefaultMaterial);
	pColliderActor->GetCollider(colliderID);
	colliderInfo = pColliderActor->GetCollider(colliderID);

	/// Collider info can only be queried after the component is intitalized (Because it is both part of the scene AND gameobject)
	colliderInfo.SetTrigger(true);

	// Attach Trigger Callback to owning Gameobject
	m_pRightGoal->SetOnTriggerCallBack(
	[=](GameObject*, GameObject* pOther, PxTriggerAction action)
	{
		 if (pOther == m_pBall)
		 {
			 if (action == PxTriggerAction::ENTER)
			 {
				 m_DirectionScale = -1.f;
				 ++m_Player1Score;
				 SetupScene();
			 }
		}
	});
	m_pRightGoal->GetTransform()->Translate(35, 0, 0);
#pragma endregion

#pragma region Players
	// Player1
	m_pPlayer1 = AddChild(new CubePrefab(2.f, 2, m_PlayerLength, XMFLOAT4{ Colors::Red }));
	auto pPlayerActor = m_pPlayer1->AddComponent(new RigidBodyComponent(true));
	pPlayerActor->AddCollider(PxBoxGeometry{ 1.f, 1.f, m_PlayerLength * 0.5f }, *pDefaultMaterial);

	// Player2
	m_pPlayer2 = AddChild(new CubePrefab(2.f, 2, m_PlayerLength, XMFLOAT4{ Colors::Blue }));
	pPlayerActor = m_pPlayer2->AddComponent(new RigidBodyComponent(true));
	pPlayerActor->AddCollider(PxBoxGeometry{ 1.f, 1.f, m_PlayerLength * 0.5f }, *pDefaultMaterial);

#pragma endregion

#pragma region Ball
	// White Sphere
	m_pBall = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::White }));
	auto pSphereActor = m_pBall->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, * pDefaultMaterial);

	auto pJoint = PxD6JointCreate(physx,
		nullptr, PxTransform{ 0.f, 0.f, 0.f },
		m_pBall->GetComponent< RigidBodyComponent>()->GetPxRigidActor(), PxTransform{ 0,0,0 });
	pJoint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	pJoint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);
#pragma endregion



#pragma region Input_Actions
	// input actions
	InputAction actionReset{ int(InputIDs::Reset), InputState::released,'R', -1 };
	InputAction actionFire{ int(InputIDs::Start), InputState::released, VK_SPACE, -1 };
	InputAction P2MoveUp{ int(InputIDs::P1MoveUp), InputState::down, 'Q', -1 };
	InputAction P2MoveDown{ int(InputIDs::P1MoveDown), InputState::down, 'A', -1 };
	InputAction P1MoveUp{ int(InputIDs::P2MoveUp), InputState::down, VK_UP, -1 };
	InputAction P1MoveDown{ int(InputIDs::P2MoveDown), InputState::down, VK_DOWN, -1 };
	GetSceneContext().pInput->AddInputAction(actionReset);
	GetSceneContext().pInput->AddInputAction(actionFire);
	GetSceneContext().pInput->AddInputAction(P1MoveUp);
	GetSceneContext().pInput->AddInputAction(P1MoveDown);
	GetSceneContext().pInput->AddInputAction(P2MoveUp);
#pragma endregion



	FullResetScene();
}

void W3_PongScene::Update()
{
	// RESET OPTION
	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::Reset)))
	{
		FullResetScene();
	}

	// Controlls
	constexpr float velocity{ 25.f };
	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::Start)))
	{
		if (!m_isStarted)
		{
			float zVelocity = (float)(rand() % (-5 - 5 + 1)) + 5;
			m_pBall->GetComponent<RigidBodyComponent>()->AddForce({ m_DirectionScale * velocity,0,zVelocity }, PxForceMode::eIMPULSE);
			m_isStarted = true;
		}
	}


	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::P1MoveUp)))
	{
		DirectX::XMFLOAT3 currentPos = m_pPlayer1->GetTransform()->GetPosition();
		if (currentPos.z + m_PlayerLength * 0.5f < m_UpperBound)
		{
			currentPos.z += velocity * GetSceneContext().pGameTime->GetElapsed();
			m_pPlayer1->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
		}
			
		
	}
	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::P1MoveDown)))
	{
		DirectX::XMFLOAT3 currentPos = m_pPlayer1->GetTransform()->GetPosition();
		if (currentPos.z - m_PlayerLength * 0.5f > m_LowerBound)
		{
			currentPos.z -= velocity * GetSceneContext().pGameTime->GetElapsed();
			m_pPlayer1->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
		}
			
	}
	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::P2MoveUp)))
	{
		DirectX::XMFLOAT3 currentPos = m_pPlayer2->GetTransform()->GetPosition();
		if (currentPos.z + m_PlayerLength * 0.5f < m_UpperBound)
		{
			currentPos.z += velocity * GetSceneContext().pGameTime->GetElapsed();
			m_pPlayer2->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
		}
	}
	if (GetSceneContext().pInput->IsActionTriggered(int(InputIDs::P2MoveDown)))
	{
		DirectX::XMFLOAT3 currentPos = m_pPlayer2->GetTransform()->GetPosition();
		if (currentPos.z - m_PlayerLength * 0.5f > m_LowerBound)
		{
			currentPos.z -= velocity * GetSceneContext().pGameTime->GetElapsed();
			m_pPlayer2->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
		}
	}
}

void W3_PongScene::OnSceneActivated()
{
	FullResetScene();
}

void W3_PongScene::OnGUI()
{
	if (ImGui::CollapsingHeader("Controls"))
	{
		ImGui::Text("Pong Controls:");
		ImGui::Text("");
		ImGui::Text("-> Launch Ball: [SPACE]");
		ImGui::Text("-> Left Peddle: [Q]/[A]");
		ImGui::Text("-> Right Peddle: [Up]/[Down]");
		ImGui::Text("-> Reset Game: [R]");
	}
	if (ImGui::CollapsingHeader("Extra", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("If reft peddle scores, Ball will go left");
		ImGui::Text("If right peddle scores, Ball will go right");
		ImGui::Text("-------------------");
		ImGui::Text("Ball shoots in a random z direction");
	}
	ImGui::Begin(StringUtil::utf8_encode(L"Score").c_str(), nullptr, ImGuiTreeNodeFlags_DefaultOpen);
		ImGui::Text("Current Score:");
		ImGui::Text("Left Peddle\t-\tRight Peddle");
		ImGui::Text("\t\t%d\t\t\t\t-\t\t\t%d", m_Player1Score, m_Player2Score);
	ImGui::End();
}

void W3_PongScene::SetupScene()
{

	m_isStarted = false;
	m_pBall->GetTransform()->Translate(0.f, 0.f, 0.f);
	m_pBall->GetTransform()->Rotate(0.f, 0.f, 0.f);
}

void W3_PongScene::FullResetScene()
{
	m_pPlayer1->GetTransform()->Translate(-30.f, 0.f, 0.f);
	m_pPlayer2->GetTransform()->Translate(30.f, 0.f, 0.f);
	m_Player1Score = 0;
	m_Player2Score = 0;
	SetupScene();
}
