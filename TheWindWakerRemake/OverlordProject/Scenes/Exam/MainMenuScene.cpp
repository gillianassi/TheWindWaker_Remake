#include "stdafx.h"
#include "MainMenuScene.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/ColorMaterial.h"
#include "Materials/Water/FoamMaterial.h"
#include "Materials/Water/ShoreWaterMaterial.h"
#include "Materials/Misc/SkyboxMaterial.h"

#include "Materials/Post/PostFade.h"

#include "Prefabs/Link.h"
#include "Prefabs/OutsetIsland.h"
#include "Prefabs/Rupees/RupeeBlue.h"
#include "Prefabs/Rupees/RupeeYellow.h"
#include "Prefabs/Rupees/RupeeGreen.h"

#include "Managers/GameSoundManager.h"


void MainMenuScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;


	auto soundManager = GameSoundManager::Get();
	soundManager->AddSound(GameSoundManager::Sound::PressStart, "Resources/Sound/SFX/WW_PressStart.wav");
	soundManager->AddSound(GameSoundManager::Sound::Menu_Quit, "Resources/Sound/SFX/WW_PauseMenu_Quit.wav");



	//Materials
	//*********

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);



	//skybox
	auto skybox = AddChild(new GameObject());
	const auto pModel = skybox->AddComponent(new ModelComponent(L"Meshes/skybox.ovm"));

	const auto pMaterial = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Skybox/FullSkybox.png");
	pModel->SetMaterial(pMaterial);
	skybox->GetTransform()->Scale(400, 350, 400);
	skybox->GetTransform()->Rotate(0, 135, 0);


	PlaceRupees();

	//Environment
	//**************

	m_pOutsetIslandObj = AddChild(new OutsetIsland());

	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	// LIGHTS
	XMFLOAT3 dir = XMFLOAT3{ 0.704f, -0.707f, -0.064f };

	XMFLOAT3 islandPos{ m_pOutsetIslandObj->GetTransform()->GetWorldPosition() };
	XMFLOAT3 lightPos{ (-dir.x * m_LightDistance) + islandPos.x, (-dir.y * m_LightDistance * 0.5f) + islandPos.y, (-dir.z * m_LightDistance) + islandPos.z };
	m_SceneContext.pLights->SetDirectionalLight(lightPos, dir);
	//m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });


	//UI
	//****
	auto mainMenu = AddChild(new GameObject());
	mainMenu->GetTransform()->Translate(m_SceneContext.windowWidth * 0.5f, m_SceneContext.windowHeight * 0.5f, .9f);


	auto title = mainMenu->AddChild(new GameObject());
	title->AddComponent(new SpriteComponent(L"Textures/UI/ZeldaWindWaker.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	title->GetTransform()->Translate(0.f, 0.f, 1.f);
	title->GetTransform()->Scale(0.5f, 0.5f, 1.f);

	auto banner = mainMenu->AddChild(new GameObject());
	banner->AddComponent(new SpriteComponent(L"Textures/DAEBanner.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	banner->GetTransform()->Translate(0.f, 0.f, 1.f);
	//banner->GetTransform()->Scale(0.5f, 0.5f, 1.f);


	auto disclaimerSprite = mainMenu->AddChild(new GameObject());
	disclaimerSprite->AddComponent(new SpriteComponent(L"Textures/UI/GillianAssi.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	disclaimerSprite->GetTransform()->Translate(0.f, -m_SceneContext.windowHeight * 0.3f, 1.f);
	disclaimerSprite->GetTransform()->Scale(0.2f, 0.2f, 1.f);

	auto startButton = mainMenu->AddChild(new GameObject());
	m_pSprite = startButton->AddComponent(new SpriteComponent(L"Textures/UI/PressStart.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	startButton->GetTransform()->Translate(0.f, m_SceneContext.windowHeight * 0.30f, 1.f);
	startButton->GetTransform()->Scale(0.5f, 0.5f, 1.f);


	//Camera
	const auto pCameraObj = AddChild(new GameObject);
	const auto pCamera = pCameraObj->AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	// set as active camera
	m_pCameraComponent->SetActive(true);
	m_pCameraComponent->GetTransform()->Translate(m_PositionVec[0]);
	m_pCameraComponent->GetTransform()->Rotate(m_RotationVec[0]);
	

	//Input
	//*****
	m_SceneContext.pInput->AddInputAction(InputAction(StartGame, InputState::pressed, VK_RETURN, -1, XINPUT_GAMEPAD_START));
	//Input
	auto inputAction = InputAction(QuitGame, InputState::down, VK_ESCAPE, -1, XINPUT_GAMEPAD_B);
	m_SceneContext.pInput->AddInputAction(inputAction);


	m_PostFade = MaterialManager::Get()->CreateMaterial<PostFade>();

	AddPostProcessingEffect(m_PostFade);
}

void MainMenuScene::OnSceneActivated()
{

	// SOUND
	//*********
	auto pSoundSystem = SoundManager::Get()->GetSystem();
	FMOD::Sound* pSound{};
	pSoundSystem->createStream("Resources/Sound/Music/WindWaker_ThemeSong.mp3", FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pSoundSystem->playSound(pSound, nullptr, false, &m_pMusicChannel);
}

void MainMenuScene::Update()
{
	HandleFade();

	//std::cout << m_pCharacter->GetTransform()->GetWorldPosition().x << ", " << m_pCharacter->GetTransform()->GetWorldPosition().y << ", " << m_pCharacter->GetTransform()->GetWorldPosition().z << ", " << std::endl;

	if (m_SceneContext.pInput->IsActionTriggered(StartGame))
	{
		m_FadeSpeed = -m_FadeSpeed;
		m_HasStarted = true;
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::PressStart);

	}
	if (m_SceneContext.pInput->IsActionTriggered(QuitGame))
	{
		m_FadeSpeed = -m_FadeSpeed;
		m_HasQuit = true;
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Quit);
	}


#pragma region ShadowMap
	// LIGHTS
	//const auto pCameraTransform =m_pCharacter->GetCameraComponent()->GetTransform();
	const auto lightDir = m_SceneContext.pLights->GetDirectionalLight().direction;
	XMFLOAT3 dir = XMFLOAT3{ lightDir.x, lightDir.y, lightDir.z };
	//m_SceneContext.pLights->SetDirectionalLight(pCameraTransform->GetWorldPosition(), dir);


#pragma endregion


	auto pos = m_pCameraComponent->GetTransform()->GetPosition();
	auto rot = m_pCameraComponent->GetTransform()->GetWorldRotation();

	//std::cout << "pos: {" << pos.x << "f, " << pos.y << "f, " << pos.z << "f}" << std::endl;
	//std::cout << "rot: {" << rot.x << ", " << rot.y << ", " << rot.z << "f}" << std::endl;

	float elapsedTime = m_SceneContext.pGameTime->GetElapsed();
	m_Weight += elapsedTime * m_CameraSpeed;


	int nextIndex = (m_CurrentIndex + 1) % (m_PositionVec.size() - 1);

	XMFLOAT3 currentPos{};
	currentPos.x = m_PositionVec[m_CurrentIndex].x * (1.f - m_Weight) + m_PositionVec[nextIndex].x * m_Weight;
	currentPos.y = m_PositionVec[m_CurrentIndex].y * (1.f - m_Weight) + m_PositionVec[nextIndex].y * m_Weight;
	currentPos.z = m_PositionVec[m_CurrentIndex].z * (1.f - m_Weight) + m_PositionVec[nextIndex].z * m_Weight;

	XMFLOAT3 currentRot{};
	currentRot.x = m_RotationVec[m_CurrentIndex].x * (1.f - m_Weight) + m_RotationVec[nextIndex].x * m_Weight;
	currentRot.y = m_RotationVec[m_CurrentIndex].y * (1.f - m_Weight) + m_RotationVec[nextIndex].y * m_Weight;
	currentRot.z = m_RotationVec[m_CurrentIndex].z * (1.f - m_Weight) + m_RotationVec[nextIndex].z * m_Weight;
	
	//std::cout << "rot: " << currentRot.x << ", " << currentRot.y << ", " << currentRot.z << std::endl;


	m_pCameraComponent->GetTransform()->Translate(currentPos);
	m_pCameraComponent->GetTransform()->Rotate(currentRot);

	if (m_Weight >= 1.f)
	{
		m_CurrentIndex = (m_CurrentIndex + 1) % (m_PositionVec.size() - 1);
		m_Weight = 0.f;
	}


	float opacity = 0.5f * std::cosf(2.f * 3.14f * m_ButtonSpeed * m_SceneContext.pGameTime->GetTotal()) + 0.5f;

	m_pSprite->SetColor( XMFLOAT4{1.f,1.f,1.f,opacity });

}

void MainMenuScene::HandleFade()
{
	if (m_FadeSpeed > 0)
	{
		if (m_CurrentFade < 1.f)
		{
			m_CurrentFade += m_SceneContext.pGameTime->GetElapsed() * m_FadeSpeed;
			m_PostFade->SetFadeWeight(m_CurrentFade);
			m_pMusicChannel->setPaused(true);
			m_pMusicChannel->setVolume(m_CurrentFade);
			m_pMusicChannel->setPaused(false);
		}
	}
	else if (m_FadeSpeed < 0)
	{
		if (m_CurrentFade > 0.f)
		{
			m_CurrentFade += m_SceneContext.pGameTime->GetElapsed() * m_FadeSpeed;
			m_PostFade->SetFadeWeight(m_CurrentFade);
			m_pMusicChannel->setPaused(true);
			m_pMusicChannel->setVolume(m_CurrentFade);
			m_pMusicChannel->setPaused(false);
		}
	}

	if (m_CurrentFade <= 0.f)
	{
		if (m_HasStarted)
		{
			m_pMusicChannel->stop();
			SceneManager::Get()->SetFromToScene(L"MainMenuScene", L"WindWakerScene");
			SceneManager::Get()->SetActiveGameScene(L"TransitionScene");
		}
		if (m_HasQuit)
		{
			m_pMusicChannel->stop();
			PostQuitMessage(0);
		}
	}
}

void MainMenuScene::PostDraw()
{
	//Draw ShadowMap (Debug Visualization)
	if (m_DrawShadowMap) {

		ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
	}




}

void MainMenuScene::OnGUI()
{
	m_pLink->DrawImGui();
}


void MainMenuScene::PlaceRupees()
{
	// Green
	auto rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(14.f, 1.25f, 9.41f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(10.43f, 1.41f, 9.27f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(-9.33f, 2.11f, 19.f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(6.02f, 1.41f, 9.35f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(16.68f, 0.769f, 13.12f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(1.615f, 1.27f, 11.41f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(-3.96f, 1.09f, 14.97f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(24.9f, 2.62f, 9.7f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(25.74f, 2.66f, 13.28f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(22.21f, 2.34f, 10.46f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(23.f, 2.28f, 7.88f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(21.256f, 2.16f, 8.74f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(26.26f, 2.61f, 14.48f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(18.53f, 0.85f, 15.59f);
	rupeeGreen = AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(20.53f, 0.85f, 16.59f);
	// blue rupees
	auto rupeeBlue = AddChild(new RupeeBlue(m_pLink));
	rupeeBlue->GetTransform()->Translate(5.66f, 1.37f, 6.39f);
	rupeeBlue = AddChild(new RupeeBlue(m_pLink));
	rupeeBlue->GetTransform()->Translate(22.97f, 2.398f, 9.255f);
	rupeeBlue = AddChild(new RupeeBlue(m_pLink));
	rupeeBlue->GetTransform()->Translate(17.7f, 1.75f, 4.94f);
	// Yellow rupees
	auto rupeeYellow = AddChild(new RupeeYellow(m_pLink));
	rupeeYellow->GetTransform()->Translate(-11.55f, 2.16f, 23.44f);
	rupeeYellow = AddChild(new RupeeYellow(m_pLink));
	rupeeYellow->GetTransform()->Translate(23.35f, 0.705f, 18.53f);
}
