#pragma once
class W3_PongScene : public GameScene
{
public:
	W3_PongScene();
	~W3_PongScene() override = default;

	W3_PongScene(const W3_PongScene& other) = delete;
	W3_PongScene(W3_PongScene&& other) noexcept = delete;
	W3_PongScene& operator=(const W3_PongScene& other) = delete;
	W3_PongScene& operator=(W3_PongScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;

	void OnSceneActivated() override;
	void OnGUI() override;

	void SetupScene();
	void FullResetScene();


private:
	enum class InputIDs
	{
		Reset,
		Start,
		P1MoveUp,
		P1MoveDown,
		P2MoveUp,
		P2MoveDown
	};

	GameObject* m_pBall;
	GameObject* m_pPlayer1;
	GameObject* m_pPlayer2;
	GameObject* m_pLeftGoal;
	GameObject* m_pRightGoal;

	bool m_isStarted;
	const float m_MaxDist;
	const float m_WallWidth;
	const float m_UpperBound;
	const float m_LowerBound;
	const float m_PlayerLength;
	int m_Player1Score;
	int m_Player2Score;

	float m_DirectionScale;
};

