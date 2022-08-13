#pragma once
class BaseWaterMaterial final : public Material<BaseWaterMaterial>
{
public:
	BaseWaterMaterial();
	~BaseWaterMaterial() override = default;

	BaseWaterMaterial(const BaseWaterMaterial& other) = delete;
	BaseWaterMaterial(BaseWaterMaterial&& other) noexcept = delete;
	BaseWaterMaterial& operator=(const BaseWaterMaterial& other) = delete;
	BaseWaterMaterial& operator=(BaseWaterMaterial&& other) noexcept = delete;

	void UseTransparency(bool enabled);
	void SetColor(const XMFLOAT4& color) const;

protected:
	void InitializeEffectVariables() override;
};

