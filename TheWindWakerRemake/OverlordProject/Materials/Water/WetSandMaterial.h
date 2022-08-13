#pragma once
class WetSandMaterial final : public Material<WetSandMaterial>
{
public:
	WetSandMaterial();
	~WetSandMaterial() override = default;

	WetSandMaterial(const WetSandMaterial& other) = delete;
	WetSandMaterial(WetSandMaterial&& other) noexcept = delete;
	WetSandMaterial& operator=(const WetSandMaterial& other) = delete;
	WetSandMaterial& operator=(WetSandMaterial&& other) noexcept = delete;

	void SetSandTexture(const std::wstring& assetFile) const;
	void SetTotalTime(float color) const;
	void SetRetractSpeed(float speed) const;
	void SetShadeOpacity(float opacity) const;
	void SetDepthBias(float bias) const;

protected:
	void InitializeEffectVariables() override;
private:
	void SetTexture(const std::wstring varName, const std::wstring& assetFile) const;
};

