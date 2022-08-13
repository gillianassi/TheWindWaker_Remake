#pragma once
class EdgeFoamMaterial final : public Material<EdgeFoamMaterial>
{
public:
	EdgeFoamMaterial();
	~EdgeFoamMaterial() override = default;

	EdgeFoamMaterial(const EdgeFoamMaterial& other) = delete;
	EdgeFoamMaterial(EdgeFoamMaterial&& other) noexcept = delete;
	EdgeFoamMaterial& operator=(const EdgeFoamMaterial& other) = delete;
	EdgeFoamMaterial& operator=(EdgeFoamMaterial&& other) noexcept = delete;

	void SetFoamColor(const XMFLOAT4& color) const;
	void SetUVSpeed(const XMFLOAT2& uvSpeed) const;
	void SetFoamTexture(const std::wstring& assetFile) const;
	void SetTotalTime(float color) const;
	void SetVSquish(float offset) const;
	void SetDepthBias(float bias) const;

protected:
	void InitializeEffectVariables() override;
private:
	void SetTexture(const std::wstring varName, const std::wstring& assetFile) const;
};

