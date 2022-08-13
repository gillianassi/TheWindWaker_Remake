#pragma once
class DiffuseMaterial_Toon final : public Material<DiffuseMaterial_Toon>
{
public:
	DiffuseMaterial_Toon();
	~DiffuseMaterial_Toon() override = default;

	DiffuseMaterial_Toon(const DiffuseMaterial_Toon& other) = delete;
	DiffuseMaterial_Toon(DiffuseMaterial_Toon&& other) noexcept = delete;
	DiffuseMaterial_Toon& operator=(const DiffuseMaterial_Toon& other) = delete;
	DiffuseMaterial_Toon& operator=(DiffuseMaterial_Toon&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetLightColor(const XMFLOAT4& color) const;
	void SetDetail(float detail) const;
	void SetStrength(float strength) const;
	void SetBrightness(float brightness) const;
protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext&, const ModelComponent*) const override;

private:
	TextureData* m_pDiffuseTexture{};
};

