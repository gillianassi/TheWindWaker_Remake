#pragma once
class Character;

class TransitionScene : public GameScene
{
public:
	TransitionScene() :GameScene(L"TransitionScene") {}
	~TransitionScene() override = default;
	TransitionScene(const TransitionScene & other) = delete;
	TransitionScene(TransitionScene && other) noexcept = delete;
	TransitionScene& operator=(const TransitionScene & other) = delete;
	TransitionScene& operator=(TransitionScene && other) noexcept = delete;

protected:

	void OnSceneActivated() override;
	void Initialize() override;

	void Update() override;

	void OnGUI() override;

private:

};

