#pragma once
class SceneManager final: public Singleton<SceneManager>
{
public:
	SceneManager(const SceneManager& other) = delete;
	SceneManager(SceneManager&& other) noexcept = delete;
	SceneManager& operator=(const SceneManager& other) = delete;
	SceneManager& operator=(SceneManager&& other) noexcept = delete;

	void AddGameScene(GameScene* pScene);

	void RemoveGameScene(const std::wstring& sceneName, bool deleteObject = false);
	void RemoveGameScene(GameScene* pScene, bool deleteObject = false);
	void SetActiveGameScene(const std::wstring& sceneName);
	void NextScene();
	void PreviousScene();
	GameScene* GetActiveScene() const { return m_ActiveScene; }
	const SceneContext& GetActiveSceneContext() const { return m_ActiveScene->GetSceneContext(); }
	SceneSettings& GetActiveSceneSettings() const { return m_ActiveScene->GetSceneSettings(); }

	void SetFromToScene(const std::wstring& from, const std::wstring& to)
	{
		m_FromScene = from;
		m_ToScene = to;
	}

	const std::wstring& GetFromScene()
	{
		return m_FromScene;
	}

	const std::wstring& GetToScene()
	{
		return m_ToScene;
	}

protected:
	void Initialize() override;

private:
	friend class OverlordGame;
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	~SceneManager();

	void PostInitialize() const;
	void WindowStateChanged(int state, bool active) const;
	void Update();
	void Draw() const;
	void OnGUI() const;

	std::vector<GameScene*> m_pScenes{};
	GameScene* m_ActiveScene{}, * m_NewActiveScene{};


	std::wstring m_FromScene{}, m_ToScene{};
};

