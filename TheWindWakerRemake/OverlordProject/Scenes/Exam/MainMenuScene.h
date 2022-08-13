#pragma once
class Link;
class OutsetIsland;
class FoamMaterial;
class ShoreWaterMaterial;
class PostFade;
class MainMenuScene final : public GameScene
{
	
public:
	MainMenuScene() :GameScene(L"MainMenuScene") {}
	~MainMenuScene() override = default;
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene && other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene & other) = delete;
	MainMenuScene& operator=(MainMenuScene && other) noexcept = delete;

protected:
	void Initialize() override;
	void OnSceneActivated() override;
	void PlaceRupees();
	void Update() override;
	void HandleFade();
	void PostDraw() override;
	void OnGUI() override;

private:
	enum InputIds
	{
		StartGame,
		QuitGame
	};

	OutsetIsland* m_pOutsetIslandObj{};
	Link* m_pLink{};
	bool m_DrawShadowMap{ false };
	float m_ShadowMapScale{ 0.3f };
	float m_LightDistance{ 4.f };

	CameraComponent* m_pCameraComponent{};

	SpriteComponent* m_pSprite{};
	SpriteFont* m_pFont{};
	SpriteFont* m_pFontOutline{};
	XMFLOAT2 m_TextPosition{1080,600};
	XMFLOAT2 m_TextPosition2{1078,600};
	XMFLOAT4 m_TextColor{ 0.76f,0.76f,0.37f,1.f };

	std::vector<XMFLOAT3> m_PositionVec =
	{
		{46.0336f, 16.5066f, 17.5303f},
		{33.7278f, 11.1241f, 33.3267f},
		{11.2003f, 6.55798f, 27.4854f},
		{-16.1437f, 6.29951f, 28.3375f},
		{-5.44988f, 2.34096f, 15.1862f},
		{14.7342f, 2.54119f, 8.59757f},
		{35.2656f, 11.6625f, 7.87844f}
	};
	std::vector<XMFLOAT3> m_RotationVec =
	{
		{10.f, 280.f, 0.f},
		{10.f, 200.f, 0.f},
		{0.f, 180.f, 0.f},
		{0.f, 270.f, 0.f},
		{0.f, 270.f, 0.f},
		{10.f, 200.f, 0.f},
		{10.f, 100.f, 0.f}
	};

	PostFade* m_PostFade{};

	float m_CurrentFade{};

	float m_FadeSpeed{0.5f};

	bool m_HasQuit{ false };
	bool m_HasStarted{ false };


	int m_CurrentIndex{};
	float m_CameraSpeed{ .1f };
	float m_Weight{ 0.f };

	float m_ButtonSpeed{ 0.5f };

	FMOD::Channel* m_pMusicChannel{};
};

