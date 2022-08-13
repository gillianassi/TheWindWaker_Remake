#pragma once
#include "Character.h"

class ParticleEmitterComponent;
class Link : public Character
{
public:
	Link(const CharacterDesc& characterDesc, PhysxProxy* pPhysxScene);
	~Link() override = default;

	Link(const Link& other) = delete;
	Link(Link&& other) noexcept = delete;
	Link& operator=(const Link& other) = delete;
	Link& operator=(Link&& other) noexcept = delete;

	void DrawImGui();
	void CollectRupees(int value) { m_CollectedRupees += value; }
	int GetRupies() { return m_CollectedRupees; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	int m_CollectedRupees{};
	ParticleEmitterComponent* m_pEmitter{};
	ModelAnimator* m_pAnimator{};
};

