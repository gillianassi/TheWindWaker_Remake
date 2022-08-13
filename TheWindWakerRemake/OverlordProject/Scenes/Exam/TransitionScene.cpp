#include "stdafx.h"
#include "TransitionScene.h"
#include "WindWakerScene.h"
#include "MainMenuScene.h"

void TransitionScene::OnSceneActivated()
{
}



void TransitionScene::Initialize()
{
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;

	m_SceneContext.settings.clearColor = XMFLOAT4{ Colors::Black };
}

void TransitionScene::Update()
{
	auto sceneManager = SceneManager::Get();
	std::wstring from = sceneManager->GetFromScene();
	std::wstring to = sceneManager->GetToScene();
	
	if (from.empty() || to.empty())
		return;

	sceneManager->RemoveGameScene(from, true);

	if (from == L"WindWakerScene")
	{
		sceneManager->AddGameScene(new WindWakerScene());
	}
	else
	{
		sceneManager->AddGameScene(new MainMenuScene());
	}

	sceneManager->SetActiveGameScene(to);
}

void TransitionScene::OnGUI()
{
}
