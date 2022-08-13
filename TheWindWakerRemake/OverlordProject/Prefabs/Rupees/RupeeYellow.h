#pragma once
#include "Rupee.h"
class Link;
class RupeeYellow : public Rupee
{
public:
	RupeeYellow(Link* pLink);
	~RupeeYellow() override = default;

	RupeeYellow(const RupeeYellow & other) = delete;
	RupeeYellow(RupeeYellow && other) noexcept = delete;
	RupeeYellow& operator=(const RupeeYellow & other) = delete;
	RupeeYellow& operator=(RupeeYellow && other) noexcept = delete;

	void DrawImGui();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
};

