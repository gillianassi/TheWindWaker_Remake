#include "stdafx.h"
#include "ShadeWaterMaterial.h"

ShadeWaterMaterial::ShadeWaterMaterial() :
	Material<ShadeWaterMaterial>(L"Effects/Water/ShadeWater.fx")
{}

void ShadeWaterMaterial::InitializeEffectVariables()
{
	SetUVSpeed(XMFLOAT2{ 0.2f, 0.f });
	SetDepthBias(0.001f);
	SetShadeOpacity(0.25f);
	SetVOffset(0.f);
	SetShadeTexture(L"Textures/Sea/ShoreSmall.png");
}




void ShadeWaterMaterial::SetUVSpeed(const XMFLOAT2& uvSpeed) const
{
	const XMFLOAT3& convert = XMFLOAT3(uvSpeed.x, uvSpeed.y, 0.f);
	const std::wstring varName = L"gUVSpeed";
	SetVariable_Vector(varName, convert);
}

void ShadeWaterMaterial::SetTotalTime(float totalTime) const
{
	const std::wstring varName = L"gTotalTime";
	SetVariable_Scalar(varName, totalTime);
}

void ShadeWaterMaterial::SetDepthBias(float bias) const
{
	const std::wstring varName = L"gDepthBias";
	SetVariable_Scalar(varName, bias);
}

void ShadeWaterMaterial::SetShadeOpacity(float opacity) const
{
	const std::wstring varName = L"gShadeOpacity";
	SetVariable_Scalar(varName, opacity);
}

void ShadeWaterMaterial::SetVOffset(float offset) const
{
	const std::wstring varName = L"gVOffset";
	SetVariable_Scalar(varName, offset);
}

void ShadeWaterMaterial::SetShadeTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gShadeDiffuse";
	SetTexture(varName, assetFile);
}

void ShadeWaterMaterial::SetTexture(const std::wstring varName, const std::wstring& assetFile) const
{
	TextureData* pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(varName))
	{
		pShaderVariable->AsShaderResource()->SetResource(pDiffuseTexture->GetShaderResourceView());
		return;
	}

	Logger::LogWarning(L"Shader variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}
