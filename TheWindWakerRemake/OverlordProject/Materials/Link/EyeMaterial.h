#pragma once
class EyeMaterial : public Material<EyeMaterial>
{
public:
	EyeMaterial();
	~EyeMaterial() override = default;

	EyeMaterial(const EyeMaterial& other) = delete;
	EyeMaterial(EyeMaterial&& other) noexcept = delete;
	EyeMaterial& operator=(const EyeMaterial& other) = delete;
	EyeMaterial& operator=(EyeMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const;

private:
	TextureData* m_pDiffuseTexture{};
};

