#pragma once
#include "Rupee.h"
class Link;
class RupeeGreen : public Rupee
{
public:
	RupeeGreen(Link* pLink);
	~RupeeGreen() override = default;

	RupeeGreen(const RupeeGreen & other) = delete;
	RupeeGreen(RupeeGreen && other) noexcept = delete;
	RupeeGreen& operator=(const RupeeGreen & other) = delete;
	RupeeGreen& operator=(RupeeGreen && other) noexcept = delete;

	void DrawImGui();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
};

