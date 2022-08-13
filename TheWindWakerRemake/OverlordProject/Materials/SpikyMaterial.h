#pragma once
class SpikyMaterial final: public Material<SpikyMaterial>
{
public:
	SpikyMaterial();
	~SpikyMaterial() override = default;

	void SetLightDirection(const XMFLOAT3& dir);

	SpikyMaterial(const SpikyMaterial & other) = delete;
	SpikyMaterial(SpikyMaterial && other) noexcept = delete;
	SpikyMaterial& operator=(const SpikyMaterial & other) = delete;
	SpikyMaterial& operator=(SpikyMaterial && other) noexcept = delete;

	void SetDiffuseColor(const XMFLOAT3 & color) const;
	void SetSpikeLength(const float& length) const;

protected:
	void InitializeEffectVariables() override;
};

