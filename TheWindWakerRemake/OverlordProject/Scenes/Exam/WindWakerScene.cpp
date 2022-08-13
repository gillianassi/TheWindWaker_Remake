#include "stdafx.h"
#include "WindWakerScene.h"

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

void WindWakerScene::Initialize()
{

	auto soundManager = GameSoundManager::Get();
	soundManager->AddSound(GameSoundManager::Sound::Menu_Back, "Resources/Sound/SFX/WW_PauseMenu_Back.wav");
	soundManager->AddSound(GameSoundManager::Sound::Menu_Close, "Resources/Sound/SFX/WW_PauseMenu_Close.wav");
	soundManager->AddSound(GameSoundManager::Sound::Menu_Cursor, "Resources/Sound/SFX/WW_PauseMenu_Select.wav");
	soundManager->AddSound(GameSoundManager::Sound::Menu_Open, "Resources/Sound/SFX/WW_PauseMenu_Open.wav");
	soundManager->AddSound(GameSoundManager::Sound::Menu_Quit, "Resources/Sound/SFX/WW_PauseMenu_Quit.wav");
	soundManager->AddSound(GameSoundManager::Sound::Menu_Select, "Resources/Sound/SFX/WW_PauseMenu_Select.wav");
	soundManager->AddSound(GameSoundManager::Sound::winner, "Resources/Sound/SFX/WW_Fanfare_Item.wav");

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.showInfoOverlay = false;



	//Materials
	//*********

		//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);


	//skybox
	auto pSkyBox = AddChild(new GameObject());
	const auto pModel = pSkyBox->AddComponent(new ModelComponent(L"Meshes/skybox.ovm"));

	const auto pMaterial = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Skybox/FullSkybox.png");
	pModel->SetMaterial(pMaterial);
	pSkyBox->GetTransform()->Scale(400, 350, 400);
	pSkyBox->GetTransform()->Rotate(0, 135, 0);



#pragma region UI
	//UI
	//****
	auto mainUI = AddChild(new GameObject());
	mainUI->GetTransform()->Translate(m_SceneContext.windowWidth * 0.5f, m_SceneContext.windowHeight * 0.5f, .9f);

	m_pControls = mainUI->AddChild(new GameObject());
	m_pControls->AddComponent(new SpriteComponent(L"Textures/UI/ControlsMenu.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	m_pControls->GetTransform()->Translate(0, 0, .9f);
	m_pControls->GetTransform()->Scale(.8f, .8f, 1.f);

	m_pSprite = mainUI->AddChild(new GameObject());
	m_pSprite->AddComponent(new SpriteComponent(L"Textures/UI/Rupee.png", { 0.5f,0.5f }, { 0.23f,1.f,0.f,1.f }));
	m_pSprite->GetTransform()->Translate(m_SceneContext.windowWidth * 0.3f, m_SceneContext.windowHeight * 0.38f, .9f);
	m_pSprite->GetTransform()->Scale(0.3f, 0.3f, 0.5f);

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/WindWaker_64.fnt");
	m_pFontOutline = ContentManager::Load<SpriteFont>(L"SpriteFonts/WindWaker_64_Bold.fnt");


	// pause
	m_pPauseMenu = AddChild(new GameObject());
	m_pPauseMenu->GetTransform()->Translate(m_SceneContext.windowWidth * 0.5f, m_SceneContext.windowHeight * 0.5f, .9f);

	auto backrgoundBox = m_pPauseMenu->AddChild(new GameObject());
	backrgoundBox->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Box.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	backrgoundBox->GetTransform()->Translate(0, 0, 0.95f);
	//backrgoundBox->GetTransform()->Scale(m_LargeButtonScale);

	m_pContinue = m_pPauseMenu->AddChild(new GameObject());
	m_pContinue->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Continue.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	m_pContinue->GetTransform()->Translate(0, -m_SceneContext.windowHeight * 0.2f, .95f);
	m_pContinue->GetTransform()->Scale(m_LargeButtonScale, m_LargeButtonScale, 1.f);

	m_pBack = m_pPauseMenu->AddChild(new GameObject());
	m_pBack->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Back.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	m_pBack->GetTransform()->Translate(0, 0, .95f);
	m_pBack->GetTransform()->Scale(m_NormalButtonScale, m_NormalButtonScale, 1.f);

	m_pQuit = m_pPauseMenu->AddChild(new GameObject());
	m_pQuit->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Quit.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	m_pQuit->GetTransform()->Translate(0, m_SceneContext.windowHeight * 0.2f, .95f);
	m_pQuit->GetTransform()->Scale(m_NormalButtonScale, m_NormalButtonScale, 1.f);

	m_pQuitPrompt = AddChild(new GameObject());
	m_pQuitPrompt->GetTransform()->Translate(m_SceneContext.windowWidth * 0.5f, m_SceneContext.windowHeight * 0.5f, 1.f);

	auto backrgoundBox2 = m_pQuitPrompt->AddChild(new GameObject());
	backrgoundBox2->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Box.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	backrgoundBox2->GetTransform()->Translate(0, 0, .97f);
	backrgoundBox2->GetTransform()->Scale(0.7f, 0.7f, 1.f);

	auto pPromptMessage = m_pQuitPrompt->AddChild(new GameObject());
	pPromptMessage->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Sure.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	pPromptMessage->GetTransform()->Translate(0, -m_SceneContext.windowHeight * 0.2f, 1.f);
	pPromptMessage->GetTransform()->Scale(.5f, 0.5f, 1.f);



	m_pYes = m_pQuitPrompt->AddChild(new GameObject());
	m_pYes->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/Yes.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	m_pYes->GetTransform()->Translate(0, -m_SceneContext.windowHeight * 0.05f, 1.);
	m_pYes->GetTransform()->Scale(m_NormalButtonScale, m_NormalButtonScale, 1.f);

	m_pNo = m_pQuitPrompt->AddChild(new GameObject());
	m_pNo->AddComponent(new SpriteComponent(L"Textures/UI/PauseMenu/No.png", { .5f,.5f }, { 1.f,1.f,1.f,1.f }));
	m_pNo->GetTransform()->Translate(0, m_SceneContext.windowHeight * 0.1f, 1.f);
	m_pNo->GetTransform()->Scale(m_NormalButtonScale, m_NormalButtonScale, 1.f);

	m_pQuitPrompt->SetActive(false);
	m_pPauseMenu->SetActive(false);

#pragma endregion
	
	//Character
	//**************

	CharacterDesc characterDesc{ pDefaultMaterial, 0.1f, 0.3f, 0.001f, -.37f };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;
	characterDesc.actionId_Roll = CharacterRoll;
	characterDesc.actionId_Crouch = CharacterCrouch;
	characterDesc.maxMoveSpeed = 2.f;
	characterDesc.maxFallSpeed = 7.f;
	characterDesc.JumpSpeed = 3.f;
	characterDesc.maxCameraDist = 3.f;
	characterDesc.minCameraDist = 1.f;
	characterDesc.maxCameraHeight = 1.f;
	characterDesc.moveAccelerationTime = 0.25f;
	characterDesc.fallAccelerationTime = 0.9f;
	characterDesc.crawlSpeedModifier = 0.3f;
	characterDesc.rollSpeedModifier = 2.0f;

	auto character = AddChild(new GameObject());
	m_pLink = character->AddChild(new Link(characterDesc, GetPhysxProxy()));
	m_pLink->GetTransform()->Translate(29.2f, 3.2f, 20.97f);

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


	//Input
	//*****
	//Input
	m_SceneContext.pInput->AddInputAction(InputAction(PauseGame, InputState::pressed, VK_RETURN, -1, XINPUT_GAMEPAD_START));
	
	m_SceneContext.pInput->AddInputAction(InputAction(Confirm, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A));
	
	m_SceneContext.pInput->AddInputAction(InputAction(Cancel, InputState::pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_B));
	
	m_SceneContext.pInput->AddInputAction(InputAction(MenuUp, InputState::pressed, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP));
	
	m_SceneContext.pInput->AddInputAction(InputAction(MenuDown, InputState::pressed, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN));



	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(CharacterRoll, InputState::pressed, VK_LSHIFT, -1, XINPUT_GAMEPAD_B);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterCrouch, InputState::down, VK_LCONTROL, -1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_SceneContext.pInput->AddInputAction(inputAction);



	// Post Processing
	m_PostFade = MaterialManager::Get()->CreateMaterial<PostFade>();

	AddPostProcessingEffect(m_PostFade);

}

void WindWakerScene::OnSceneActivated()
{
	// SOUND
	//*********
	auto pSoundSystem = SoundManager::Get()->GetSystem();
	FMOD::Sound* pSound{};
	pSoundSystem->createStream("Resources/Sound/Music/OutsetIsland.mp3", FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pSoundSystem->playSound(pSound, nullptr, false, &m_pMusicChannel);
}


void WindWakerScene::Update()
{

	HandleFade();

#pragma region ShadowMap
	// LIGHTS
	//const auto pCameraTransform =m_pCharacter->GetCameraComponent()->GetTransform();
	const auto lightDir = m_SceneContext.pLights->GetDirectionalLight().direction;
	XMFLOAT3 dir = XMFLOAT3{ lightDir.x, lightDir.y, lightDir.z };
	//XMFLOAT3 islandPos{ m_pOutsetIslandObj->GetTransform()->GetWorldPosition() };
	XMFLOAT3 islandPos{ m_pLink->GetTransform()->GetWorldPosition() };
	XMFLOAT3 lightPos{ (-dir.x * m_LightDistance) + islandPos.x, (-dir.y * m_LightDistance) + islandPos.y, (-dir.z * m_LightDistance) + islandPos.z };
	m_SceneContext.pLights->SetDirectionalLight(lightPos, dir);
	//ShadowMapRenderer::Get()->SetShadowmapScaleMultiplier(0.25f);
#pragma endregion

	if (!m_GameStarted && m_SceneContext.pGameTime->GetTotal() > 0.2f)
	{
		m_pLink->SetPaused(true);
		m_GameStarted = true;
	}


	if (m_pControls->IsActive() && m_SceneContext.pInput->IsActionTriggered(Confirm))
	{
		m_pControls->SetActive(false);
		m_pLink->SetPaused(false);
	}

	if (m_pControls->IsActive())
		return;



	if (m_SceneContext.pInput->IsActionTriggered(PauseGame))
	{
		TogglePause();
		// ensure that this goes away
		m_pQuitPrompt->SetActive(false);
		m_PromtQuit = false;

		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Close);

	}


	if (m_Paused)
	{

		if (m_PromtQuit)
		{
			HandlePrompt();
			return;
		}
		HandlePauseMenu();
		return;
	}

	int nrOfRupees = m_pLink->GetRupies();
	std::string rupees = "";
	if (nrOfRupees < 10)
	{
		rupees += "0";
	}
	rupees += std::to_string(nrOfRupees);

	std::string text = rupees + "/50";

	if (nrOfRupees == 50 && !m_HasWon)
	{
		m_HasWon = true;
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::winner);
	}

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(text), m_TextPosition2, XMFLOAT4(Colors::Black));

	TextRenderer::Get()->DrawText(m_pFontOutline, StringUtil::utf8_decode(text),m_TextPosition , m_TextColor);


}


void WindWakerScene::HandleFade()
{
	if (m_FadeSpeed > 0)
	{
		if (m_CurrentFade < 1.f)
		{
			m_CurrentFade += m_SceneContext.pGameTime->GetElapsed() * m_FadeSpeed;
			m_PostFade->SetFadeWeight(m_CurrentFade);
			m_pMusicChannel->setPaused(true);
			m_pMusicChannel->setVolume(m_CurrentFade * 0.5f);
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
			m_pMusicChannel->setVolume(m_CurrentFade * 0.5f);
			m_pMusicChannel->setPaused(false);
		}
	}

	if (m_CurrentFade <= 0.f)
	{
		if (m_HasStarted)
		{
			m_pMusicChannel->stop();
			SceneManager::Get()->SetFromToScene(L"WindWakerScene", L"MainMenuScene");
			SceneManager::Get()->SetActiveGameScene(L"TransitionScene");
		}
		if (m_HasQuit)
		{
			m_pMusicChannel->stop();
			PostQuitMessage(0);
		}
	}
}

void WindWakerScene::HandlePrompt()
{
	if (m_SceneContext.pInput->IsActionTriggered(MenuUp))
	{
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Cursor);
		--m_PromptID;
		if (m_PromptID < 0)
			m_PromptID = 1;
	}
	if (m_SceneContext.pInput->IsActionTriggered(MenuDown))
	{
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Cursor);
		++m_PromptID;
	}
	m_PromptID = m_PromptID % 2;

	m_pNo->GetTransform()->Scale(m_NormalButtonScale);
	m_pYes->GetTransform()->Scale(m_NormalButtonScale);

	switch (m_PromptID)
	{
	case 0:
		m_pNo->GetTransform()->Scale(m_LargeButtonScale);
		break;
	case 1:
		m_pYes->GetTransform()->Scale(m_LargeButtonScale);
		break;
	default:
		break;
	}



	if (m_SceneContext.pInput->IsActionTriggered(Confirm))
	{
		switch (m_PromptID)
		{
		case 0: // No
			m_PromtQuit = false;
			m_pQuitPrompt->SetActive(false);
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Select);
			break;// prompt Are you sure
		case 1: // Yes
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Back);
			if (m_SelectorID == 1) // main menu
			{

				m_FadeSpeed = -m_FadeSpeed;
				m_HasStarted = true;
			}
			else // quit
			{
				m_FadeSpeed = -m_FadeSpeed;
				m_HasQuit = true;
			}
			break;
		default:
			break;
		}
	}
	if (m_SceneContext.pInput->IsActionTriggered(Cancel))
	{
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Close);
		m_pQuitPrompt->SetActive(false);
		m_PromtQuit = false;
	}
}

void WindWakerScene::HandlePauseMenu()
{

	if (m_SceneContext.pInput->IsActionTriggered(MenuUp))
	{
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Cursor);
		--m_SelectorID;
		if (m_SelectorID < 0)
			m_SelectorID = 2;
	}
	if (m_SceneContext.pInput->IsActionTriggered(MenuDown))
	{
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Cursor);
		++m_SelectorID;
	}
	m_SelectorID = m_SelectorID % 3;


	m_pContinue->GetTransform()->Scale(m_NormalButtonScale);
	m_pBack->GetTransform()->Scale(m_NormalButtonScale);
	m_pQuit->GetTransform()->Scale(m_NormalButtonScale);

	switch (m_SelectorID)
	{
	case 0:
		m_pContinue->GetTransform()->Scale(m_LargeButtonScale);
		break;
	case 1:
		m_pBack->GetTransform()->Scale(m_LargeButtonScale);
		break;
	case 2:
		m_pQuit->GetTransform()->Scale(m_LargeButtonScale);
		break;
	default:
		break;
	}

	if (m_SceneContext.pInput->IsActionTriggered(Confirm))
	{
		switch (m_SelectorID)
		{
		case 0: // continue
			TogglePause();
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Close);
			break;// prompt Are you sure
		case 1:
		case 2:
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Select);
			m_pQuitPrompt->SetActive(true);
			m_PromptID = 0;
			m_PromtQuit = true;
			break;
		default:
			break;
		}
	}

	if (m_SceneContext.pInput->IsActionTriggered(Cancel))
	{
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Close);
		TogglePause();
	}

}


void WindWakerScene::PostDraw()
{
	//Draw ShadowMap (Debug Visualization)
	if (m_DrawShadowMap) {

		ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
	}
}

void WindWakerScene::OnGUI()
{
	m_pLink->DrawImGui();
}



void WindWakerScene::TogglePause()
{

	m_SelectorID = 0;
	m_PromptID = 1;
	m_Paused = !m_Paused;
	SetPause(m_Paused);
}
void WindWakerScene::SetPause(bool pause)
{
	if(pause)
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Open);
	else
		GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Menu_Close);

	m_pOutsetIslandObj->SetPaused(pause);
	m_pRupees->SetPaused(pause);
	m_pLink->SetPaused(pause);

	m_pPauseMenu->SetActive(pause);

}


void WindWakerScene::PlaceRupees()
{
	m_pRupees = AddChild(new GameObject());
	// Green
	auto rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(14.f, 1.25f, 9.41f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(10.43f, 1.41f, 9.27f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(-9.33f, 2.11f, 19.f);
	rupeeGreen =m_pRupees-> AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(6.02f, 1.41f, 9.35f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(16.68f, 0.769f, 13.12f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(1.615f, 1.27f, 11.41f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(-3.96f, 1.09f, 14.97f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(24.9f, 2.62f, 9.7f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(25.74f, 2.66f, 13.28f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(22.21f, 2.34f, 10.46f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(23.f, 2.28f, 7.88f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(21.256f, 2.16f, 8.74f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(26.26f, 2.61f, 14.48f);
	rupeeGreen = m_pRupees->AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(18.53f, 0.85f, 15.59f);
	rupeeGreen =m_pRupees-> AddChild(new RupeeGreen(m_pLink));
	rupeeGreen->GetTransform()->Translate(20.53f, 0.85f, 16.59f);
	// blue rupees
	auto rupeeBlue = m_pRupees->AddChild(new RupeeBlue(m_pLink));
	rupeeBlue->GetTransform()->Translate(5.66f, 1.37f, 6.39f);
	rupeeBlue = m_pRupees->AddChild(new RupeeBlue(m_pLink));
	rupeeBlue->GetTransform()->Translate(22.97f, 2.398f, 9.255f);
	rupeeBlue = m_pRupees->AddChild(new RupeeBlue(m_pLink));
	rupeeBlue->GetTransform()->Translate(17.7f, 1.75f, 4.94f);
	// Yellow rupees
	auto rupeeYellow = m_pRupees->AddChild(new RupeeYellow(m_pLink));
	rupeeYellow->GetTransform()->Translate(-11.55f, 2.16f, 23.44f);
	rupeeYellow = m_pRupees->AddChild(new RupeeYellow(m_pLink));
	rupeeYellow->GetTransform()->Translate(23.35f, 0.705f, 18.53f);
}
