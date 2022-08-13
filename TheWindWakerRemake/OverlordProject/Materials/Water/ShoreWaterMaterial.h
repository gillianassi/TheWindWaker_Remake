#pragma once
class ShoreWaterMaterial final : public Material<ShoreWaterMaterial>
{
public:
	ShoreWaterMaterial();
	~ShoreWaterMaterial() override = default;

	ShoreWaterMaterial(const ShoreWaterMaterial& other) = delete;
	ShoreWaterMaterial(ShoreWaterMaterial&& other) noexcept = delete;
	ShoreWaterMaterial& operator=(const ShoreWaterMaterial& other) = delete;
	ShoreWaterMaterial& operator=(ShoreWaterMaterial&& other) noexcept = delete;

	void UseTransparency(bool enabled);
	void SetWaterColor(const XMFLOAT4& color) const;

	void SetFoamColor(const XMFLOAT4& color) const;
	void SetUVSpeed(const XMFLOAT2& uvSpeed) const;
	void SetHighlightTexture(const std::wstring& assetFile) const;
	void SetFoamTexture(const std::wstring& assetFile) const;
	void SetTotalTime(float color) const;

protected:
	void InitializeEffectVariables() override;
private:
	void SetTexture(const std::wstring varName, const std::wstring& assetFile) const;
};

