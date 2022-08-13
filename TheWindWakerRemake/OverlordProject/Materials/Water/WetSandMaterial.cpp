#include "stdafx.h"
#include "WetSandMaterial.h"

WetSandMaterial::WetSandMaterial() :
	Material<WetSandMaterial>(L"Effects/Water/WetSand.fx")
{}

void WetSandMaterial::InitializeEffectVariables()
{
	SetRetractSpeed(0.3f);
	SetShadeOpacity(0.3f);
	SetSandTexture(L"Textures/Sea/Sand.png");
}

void WetSandMaterial::SetDepthBias(float bias) const
{
	const std::wstring varName = L"gDepthBias";
	SetVariable_Scalar(varName, bias);
}

void WetSandMaterial::SetRetractSpeed(float speed) const
{
	const std::wstring varName = L"gRetractSpeed";
	SetVariable_Scalar(varName, speed);
}

void WetSandMaterial::SetTotalTime(float totalTime) const
{
	const std::wstring varName = L"gTotalTime";
	SetVariable_Scalar(varName, totalTime);
}

void WetSandMaterial::SetShadeOpacity(float opacity) const
{
	const std::wstring varName = L"gShadeOpacity";
	SetVariable_Scalar(varName, opacity);
}


void WetSandMaterial::SetSandTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gFoamDiffuse";
	SetTexture(varName, assetFile);
}



void WetSandMaterial::SetTexture(const std::wstring varName, const std::wstring& assetFile) const
{
	TextureData* pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(varName))
	{
		pShaderVariable->AsShaderResource()->SetResource(pDiffuseTexture->GetShaderResourceView());
		return;
	}

	Logger::LogWarning(L"Shader variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}