#pragma once
class ComponentTestScene : public GameScene
{
public:
	ComponentTestScene();
	~ComponentTestScene() override = default;

	ComponentTestScene(const ComponentTestScene& other) = delete;
	ComponentTestScene(ComponentTestScene&& other) noexcept = delete;
	ComponentTestScene& operator=(const ComponentTestScene& other) = delete;
	ComponentTestScene& operator=(ComponentTestScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;

	void OnSceneActivated() override;
	void OnGUI() override;

	void SetupScene();


private:
	enum class InputIDs
	{
		Reset
	};

	GameObject* m_pBlueSphere;
	GameObject* m_pRedSphere;
	GameObject* m_pYellowSphere;
};

