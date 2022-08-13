#pragma once
class Link;
class OutsetIsland;
class FoamMaterial;
class ShoreWaterMaterial;
class PostFade;
class WindWakerScene final : public GameScene
{
	
public:
	WindWakerScene() :GameScene(L"WindWakerScene") {}
	~WindWakerScene() override = default;
	WindWakerScene(const WindWakerScene& other) = delete;
	WindWakerScene(WindWakerScene&& other) noexcept = delete;
	WindWakerScene& operator=(const WindWakerScene& other) = delete;
	WindWakerScene& operator=(WindWakerScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnSceneActivated() override;
	void PlaceRupees();
	void Update() override;
	void PostDraw() override;
	void OnGUI() override;
	
	

private:
	enum InputIds
	{
		PauseGame,
		Confirm,
		Cancel,
		MenuUp,
		MenuDown,
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterRoll,
		CharacterCrouch
	};

	void SetPause(bool);
	void TogglePause();
	void HandleFade();
	void HandlePrompt();
	void HandlePauseMenu();

	bool m_Paused{ false };

	OutsetIsland* m_pOutsetIslandObj{};
	GameObject* m_pRupees{};
	Link* m_pLink{};
	bool m_DrawShadowMap{ false };
	float m_ShadowMapScale{ 0.3f };
	float m_LightDistance{ 4.f };

	

	bool m_GameStarted{ false };

	GameObject* m_pSprite{};

	GameObject* m_pControls{};
	SpriteFont* m_pFont{};
	SpriteFont* m_pFontOutline{};

	// pause
	GameObject* m_pPauseMenu{};
	GameObject* m_pContinue{};
	GameObject* m_pBack{};
	GameObject* m_pQuit{};

	GameObject* m_pQuitPrompt{};
	GameObject* m_pNo{};
	GameObject* m_pYes{};

	float m_LargeButtonScale{ 0.3f };
	float m_NormalButtonScale{ 0.2f };

	int m_SelectorID{};
	int m_PromptID{};

	bool m_PromtQuit{ false };

	// transition

	PostFade* m_PostFade{};

	float m_CurrentFade{};

	float m_FadeSpeed{ 0.5f };

	bool m_HasQuit{ false };
	bool m_HasStarted{ false };

	bool m_HasWon{ false };

	XMFLOAT2 m_TextPosition{1080,600};
	XMFLOAT2 m_TextPosition2{1078,600};
	XMFLOAT4 m_TextColor{ 0.76f,0.76f,0.37f,1.f };

	FMOD::Channel* m_pMusicChannel{};

};

