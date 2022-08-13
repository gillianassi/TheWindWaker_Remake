#pragma once
#include "Rupee.h"
class Link;
class RupeeBlue : public Rupee
{
public:
	RupeeBlue(Link* pLink);
	~RupeeBlue() override = default;

	RupeeBlue(const RupeeBlue& other) = delete;
	RupeeBlue(RupeeBlue&& other) noexcept = delete;
	RupeeBlue& operator=(const RupeeBlue& other) = delete;
	RupeeBlue& operator=(RupeeBlue&& other) noexcept = delete;

	void DrawImGui();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
};

