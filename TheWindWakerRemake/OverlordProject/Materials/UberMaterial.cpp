#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial():
	Material<UberMaterial>(L"Effects/UberShader.fx"),
	m_pDiffuseTexture{nullptr},
	m_pNormalTexture{nullptr},
	m_pSpeculaeTexture{nullptr},
	m_pEnvironmentCube{nullptr},
	m_pOpacityMap{nullptr}
{
}


void UberMaterial::SetLightDirection(const XMFLOAT3& dir)
{
	const std::wstring varName = L"gLightDirection";
	SetVariable_Vector(varName, dir);
}

void UberMaterial::SetUseDiffuseTexture(const bool& enabled)
{

	const std::wstring varName = L"gUseTextureDiffuse";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	const std::wstring varName = L"gTextureDiffuse";
	SetVariable_Texture(varName,m_pDiffuseTexture);
}

void UberMaterial::SetDiffuseColor(const XMFLOAT4& color)
{
	const std::wstring varName = L"gColorDiffuse";
	SetVariable_Vector(varName, color);
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);

	const std::wstring varName = L"gTextureNormal";
	SetVariable_Texture(varName, m_pNormalTexture);
}

void UberMaterial::SetUseNormalMapping(const bool& enabled)
{
	const std::wstring varName = L"gUseTextureNormal";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetFlipGreenChannel(const bool& flip)
{
	const std::wstring varName = L"gFlipGreenChannel";
	SetVariable_Scalar(varName, flip);
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpeculaeTexture = ContentManager::Load<TextureData>(assetFile);

	const std::wstring varName = L"gTextureSpecularIntensity";
	SetVariable_Texture(varName, m_pSpeculaeTexture);
}

void UberMaterial::SetUseSpecularLighting(const bool& enabled)
{
	const std::wstring varName = L"gUseSpecular";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetUseSpecularLevelTexture(const bool& enabled)
{
	const std::wstring varName = L"gUseTextureSpecularIntensity";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetSpecularColor(const XMFLOAT4& color)
{
	const std::wstring varName = L"gColorSpecular";
	SetVariable_Vector(varName, color);
}

void UberMaterial::SetShininess(const float& shininess)
{
	const std::wstring varName = L"gShininess";
	SetVariable_Scalar(varName, shininess);
}

void UberMaterial::SetUseSpecularBlinn(const bool& enabled)
{
	const std::wstring varName = L"gUseSpecularBlinn";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetUseSpecularPhong(const bool& enabled)
{
	const std::wstring varName = L"gUseSpecularPhong";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetAmbientColor(const XMFLOAT4& color)
{
	const std::wstring varName = L"gColorAmbient";
	SetVariable_Vector(varName, color);
}

void UberMaterial::SetAmbientIntensity(const float& intensity)
{
	const std::wstring varName = L"gAmbientIntensity";
	SetVariable_Scalar(varName, intensity);
}

void UberMaterial::SetEnvironmentCube(const std::wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);

	const std::wstring varName = L"gCubeEnvironment";
	SetVariable_Texture(varName, m_pEnvironmentCube);
}

void UberMaterial::SetUseEnvironmentMapping(const bool& enabled)
{
	const std::wstring varName = L"gUseEnvironmentMapping";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetReflectionStrength(const float& strength)
{
	const std::wstring varName = L"gReflectionStrength";
	SetVariable_Scalar(varName, strength);
}

void UberMaterial::SetRefractionStrength(const float& strength)
{
	const std::wstring varName = L"gRefractionStrength";
	SetVariable_Scalar(varName, strength);
}

void UberMaterial::SetRefractionIndex(const float& index)
{
	const std::wstring varName = L"gRefractionIndex";
	SetVariable_Scalar(varName, index);
}

void UberMaterial::SetUseFresnel(const bool& enabled)
{
	const std::wstring varName = L"gUseFresnelFalloff";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetFresnelColor(const XMFLOAT4& color)
{
	const std::wstring varName = L"gColorFresnel";
	SetVariable_Vector(varName, color);
}

void UberMaterial::SetFresnelPower(const float& power)
{
	const std::wstring varName = L"gFresnelPower";
	SetVariable_Scalar(varName, power);
}

void UberMaterial::SetFresnelMultiplier(const float& multiplier)
{
	const std::wstring varName = L"gFresnelMultiplier";
	SetVariable_Scalar(varName, multiplier);
}

void UberMaterial::SetFresnelHardness(const float& hardness)
{
	const std::wstring varName = L"gFresnelHardness";
	SetVariable_Scalar(varName, hardness);
}

void UberMaterial::SetOpacityMap(const std::wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);

	const std::wstring varName = L"gTextureOpacity";
	SetVariable_Texture(varName, m_pOpacityMap);
}

void UberMaterial::SetUseOpacityMap(const bool& enabled)
{
	const std::wstring varName = L"gUseTextureOpacity";
	SetVariable_Scalar(varName, enabled);
}

void UberMaterial::SetOpacity(const float& intensity)
{
	const std::wstring varName = L"gOpacityIntensity";
	SetVariable_Scalar(varName, intensity);
}

void UberMaterial::InitializeEffectVariables()
{


	UseTransparency(true);
	SetLightDirection({0.551f,-0.534f,0.641f});
	//// Diffuse
	SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	SetUseDiffuseTexture(true);
	SetDiffuseColor({ 0.7f,0.f,1.f,1.f });

	//// Normal
	SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	SetUseNormalMapping(true);
	SetFlipGreenChannel(true);
	//// Specular
	SetSpecularTexture(L"Textures/Skulls_Heightmap.tga");
	SetUseSpecularLighting(true);
	SetUseSpecularLevelTexture(true);
	SetSpecularColor({ 0.f,1.f,0.f,1.f });
	SetShininess(40.f);
	SetUseSpecularBlinn(false);
	SetUseSpecularPhong(true);
	// Ambient
	SetAmbientColor({ 0.f,0.f,0.f,1.f });
	SetAmbientIntensity(0.f);
	//// Environment
	SetEnvironmentCube(L"Textures/Sunol_Cubemap.dds");
	SetUseEnvironmentMapping(true);
	SetReflectionStrength(0.8f);
	SetRefractionStrength(0.5f);
	SetRefractionIndex(0.9f);
	// Fresnel
	SetUseFresnel(true);
	SetFresnelColor({ 1.f,1.f,1.f,1.f });
	SetFresnelPower(2.f);
	SetFresnelMultiplier(1.5f);
	SetFresnelHardness(2.f);
	//// Opacity
	SetOpacityMap(L"Textures/Skulls_Diffusemap.tga");
	SetUseOpacityMap(false);
	SetOpacity(1.f);
}

void UberMaterial::DisplayWrongVariableNameWarning(const std::wstring& varName)
{
	Logger::LogWarning(L"boolean variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}

void UberMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"WithAlphaBlending" : L"WithoutAlphaBlending");
}
