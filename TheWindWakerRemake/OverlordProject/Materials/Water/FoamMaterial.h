#pragma once
class FoamMaterial final : public Material<FoamMaterial>
{
public:
	FoamMaterial();
	~FoamMaterial() override = default;

	FoamMaterial(const FoamMaterial& other) = delete;
	FoamMaterial(FoamMaterial&& other) noexcept = delete;
	FoamMaterial& operator=(const FoamMaterial& other) = delete;
	FoamMaterial& operator=(FoamMaterial&& other) noexcept = delete;

	void UseShoreFoam(bool enabled);
	void UseGradientOpacity(bool enabled) const;
	void SetFoamColor(const XMFLOAT4& color) const;
	void SetUVSpeed(const XMFLOAT2& uvSpeed) const;
	void SetGradientTexture(const std::wstring& assetFile) const;
	void SetFoamTexture(const std::wstring& assetFile) const;
	void SetDepthBias(float bias) const;
	void SetTotalTime(float color) const;
	void SetVOffset(float offset) const;

protected:
	void InitializeEffectVariables() override;
private:
	void SetTexture(const std::wstring varName, const std::wstring& assetFile) const;
};

