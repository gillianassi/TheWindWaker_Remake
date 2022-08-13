#pragma once
class DiffuseMaterial_Toon_Skinned : public Material<DiffuseMaterial_Toon_Skinned>
{
public:
	DiffuseMaterial_Toon_Skinned();
	~DiffuseMaterial_Toon_Skinned() override = default;

	DiffuseMaterial_Toon_Skinned(const DiffuseMaterial_Toon_Skinned& other) = delete;
	DiffuseMaterial_Toon_Skinned(DiffuseMaterial_Toon_Skinned&& other) noexcept = delete;
	DiffuseMaterial_Toon_Skinned& operator=(const DiffuseMaterial_Toon_Skinned& other) = delete;
	DiffuseMaterial_Toon_Skinned& operator=(DiffuseMaterial_Toon_Skinned&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

	void SetLightColor(const XMFLOAT4& color) const;
	void SetDetail(float detail) const;
	void SetStrength(float strength) const;
	void SetBrightness(float brightness) const;

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const override;

private:
	TextureData* m_pDiffuseTexture{};
};

