#pragma once
class SpikyMaterial;
class SpikyScene : public GameScene
{
public:
	SpikyScene();
	~SpikyScene() override = default;

	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

	void SetupScene();
private:
	GameObject* m_pSpikyBall;
	SpikyMaterial* m_pSpikyMaterial;
	float m_Angle;
};

