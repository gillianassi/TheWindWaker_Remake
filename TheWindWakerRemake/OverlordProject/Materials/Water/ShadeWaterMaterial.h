#pragma once
class ShadeWaterMaterial final : public Material<ShadeWaterMaterial>
{
public:
	ShadeWaterMaterial();
	~ShadeWaterMaterial() override = default;

	ShadeWaterMaterial(const ShadeWaterMaterial& other) = delete;
	ShadeWaterMaterial(ShadeWaterMaterial&& other) noexcept = delete;
	ShadeWaterMaterial& operator=(const ShadeWaterMaterial& other) = delete;
	ShadeWaterMaterial& operator=(ShadeWaterMaterial&& other) noexcept = delete;

	void SetUVSpeed(const XMFLOAT2& uvSpeed) const;
	void SetShadeTexture(const std::wstring& assetFile) const;
	void SetTotalTime(float totalTime) const;
	void SetDepthBias(float bias) const;
	void SetShadeOpacity(float opacity) const;
	void SetVOffset(float offset) const;

protected:
	void InitializeEffectVariables() override;
private:
	void SetTexture(const std::wstring varName, const std::wstring& assetFile) const;
};

