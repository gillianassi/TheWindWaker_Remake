#include "stdafx.h"
#include "ShoreWaterMaterial.h"

ShoreWaterMaterial::ShoreWaterMaterial() :
	Material<ShoreWaterMaterial>(L"Effects/Water/ShoreWater.fx")
{}

void ShoreWaterMaterial::InitializeEffectVariables()
{
	UseTransparency(true);
	SetHighlightTexture(L"Textures/Sea/ShoreTexture.png");
	SetFoamTexture(L"Textures/Sea/Shore3.png");
}

void ShoreWaterMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"TransparencyTech" : L"Default");
}
void ShoreWaterMaterial::SetWaterColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gColor";
	SetVariable_Vector(varName, color);
}

void ShoreWaterMaterial::SetFoamColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gFoamColor";
	SetVariable_Vector(varName, color);
}

void ShoreWaterMaterial::SetUVSpeed(const XMFLOAT2& uvSpeed) const
{
	const XMFLOAT3& convert = XMFLOAT3(uvSpeed.x, uvSpeed.y, 0.f);
	const std::wstring varName = L"gUVSpeed";
	SetVariable_Vector(varName, convert);
}

void ShoreWaterMaterial::SetTotalTime(float totalTime) const
{
	const std::wstring varName = L"gTotalTime";
	SetVariable_Scalar(varName, totalTime);
}

void ShoreWaterMaterial::SetHighlightTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gWaterHighlight";
	SetTexture(varName, assetFile);

}

void ShoreWaterMaterial::SetFoamTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gFoamDiffuse";
	SetTexture(varName, assetFile);
}



void ShoreWaterMaterial::SetTexture(const std::wstring varName, const std::wstring& assetFile) const
{
	TextureData* pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(varName))
	{
		pShaderVariable->AsShaderResource()->SetResource(pDiffuseTexture->GetShaderResourceView());
		return;
	}

	Logger::LogWarning(L"Shader variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}