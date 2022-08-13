#pragma once

class Link;
class Rupee : public GameObject
{
public:
	Rupee(Link* pLink);
	~Rupee() override = default;

	Rupee(const Rupee & other) = delete;
	Rupee(Link && other) noexcept = delete;
	Rupee& operator=(const Rupee & other) = delete;
	Rupee& operator=(Rupee && other) noexcept = delete;

	void DrawImGui();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	Link* m_pLink{};
	XMFLOAT4 m_RupeeColor{ 1.f,1.f,1.f,1.f };
	int m_RupeeValue{ 1 };
	bool m_isCollected{ false };

private:
	GameObject* m_pTriggerBox{};
	GameObject* m_pMesh{};
	float m_Rotation{ 0 };
	int m_direction{ 1 };


};

