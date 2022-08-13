#pragma once

class FoamMaterial;
class EdgeFoamMaterial;
class ShoreWaterMaterial;
class ShadeWaterMaterial;
class WetSandMaterial;
class OutsetIsland : public GameObject
{
public:
	OutsetIsland();
	~OutsetIsland() override = default;

	OutsetIsland(const OutsetIsland& other) = delete;
	OutsetIsland(OutsetIsland&& other) noexcept = delete;
	OutsetIsland& operator=(const OutsetIsland& other) = delete;
	OutsetIsland& operator=(OutsetIsland&& other) noexcept = delete;

	void DrawImGui();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:

	const FoamMaterial* m_pFoammaterialOuter{};
	FoamMaterial* m_pFoammaterialInner{};
	EdgeFoamMaterial* m_pEdgeFoam{};

	const ShoreWaterMaterial* m_pShoreWaterMaterial{};
	const WetSandMaterial* m_pWetSandMaterial{};
	const ShadeWaterMaterial* m_pWaterShade{};
};

