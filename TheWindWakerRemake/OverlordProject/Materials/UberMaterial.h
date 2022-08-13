#pragma once
class UberMaterial final : public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	void UseTransparency(bool enabled);

	void SetLightDirection(const XMFLOAT3 & dir);

	// Diffuse
	void SetUseDiffuseTexture(const bool& enabled);
	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetDiffuseColor(const XMFLOAT4 & color);

	// Normal
	void SetNormalTexture(const std::wstring& assetFile);
	void SetUseNormalMapping(const bool& enabled);
	void SetFlipGreenChannel(const bool& flip);

	// Specular
	void SetSpecularTexture(const std::wstring& assetFile);
	void SetUseSpecularLighting(const bool& enabled);
	void SetUseSpecularLevelTexture(const bool& enabled);
	void SetSpecularColor(const XMFLOAT4 & color);
	void SetShininess(const float& shininess);
	void SetUseSpecularBlinn(const bool& enabled);
	void SetUseSpecularPhong(const bool& enabled);
	// Ambient
	void SetAmbientColor(const XMFLOAT4 & color);
	void SetAmbientIntensity(const float& intensity);
	// Environment
	void SetEnvironmentCube(const std::wstring& assetFile);
	void SetUseEnvironmentMapping(const bool& enabled);
	void SetReflectionStrength(const float& strength);
	void SetRefractionStrength(const float& strength);
	void SetRefractionIndex(const float& index);
	// Fresnel
	void SetUseFresnel(const bool& enabled);
	void SetFresnelColor(const XMFLOAT4 & color);
	void SetFresnelPower(const float& power);
	void SetFresnelMultiplier(const float& multiplier);
	void SetFresnelHardness(const float& hardness);
	// Opacity
	void SetOpacityMap(const std::wstring& assetFile);
	void SetUseOpacityMap(const bool& enabled);
	void SetOpacity(const float& intensity);


protected:
	void InitializeEffectVariables() override;

private:

	void DisplayWrongVariableNameWarning(const std::wstring& varName);

	// Diffuse
	TextureData* m_pDiffuseTexture;

	// Normal
	TextureData* m_pNormalTexture;

	// Specular
	TextureData* m_pSpeculaeTexture;

	// Environment
	TextureData* m_pEnvironmentCube;

	// Opacity
	TextureData* m_pOpacityMap;
};

