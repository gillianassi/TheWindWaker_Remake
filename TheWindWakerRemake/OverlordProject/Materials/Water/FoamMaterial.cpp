#include "stdafx.h"
#include "FoamMaterial.h"

FoamMaterial::FoamMaterial() :
	Material<FoamMaterial>(L"Effects/Water/Foam.fx")
{}

void FoamMaterial::InitializeEffectVariables()
{
	UseShoreFoam(false);
	UseGradientOpacity(true);
	SetGradientTexture(L"Textures/Sea/Gradient.png");
	SetFoamTexture(L"Textures/Sea/Shore2.png");
}

void FoamMaterial::UseShoreFoam(bool enabled)
{
	SetTechnique(enabled ? L"ShoreTech" : L"Default");
}
void FoamMaterial::UseGradientOpacity(bool enabled) const
{
	const std::wstring varName = L"gUseGradientOpacity";
	SetVariable_Scalar(varName, (int)enabled);
}
void FoamMaterial::SetFoamColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gFoamColor";
	SetVariable_Vector(varName, color);
}


void FoamMaterial::SetDepthBias(float bias) const
{
	const std::wstring varName = L"gDepthBias";
	SetVariable_Scalar(varName, bias);
}


void FoamMaterial::SetVOffset(float offset) const
{
	const std::wstring varName = L"gVOffset";
	SetVariable_Scalar(varName, offset);
}

void FoamMaterial::SetUVSpeed(const XMFLOAT2& uvSpeed) const
{
	const XMFLOAT3& convert = XMFLOAT3(uvSpeed.x, uvSpeed.y, 0.f);
	const std::wstring varName = L"gUVSpeed";
	SetVariable_Vector(varName, convert);
}

void FoamMaterial::SetTotalTime(float totalTime) const
{
	const std::wstring varName = L"gTotalTime";
	SetVariable_Scalar(varName, totalTime);
}

void FoamMaterial::SetGradientTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gGradient";
	SetTexture(varName, assetFile);

}

void FoamMaterial::SetFoamTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gFoamDiffuse";
	SetTexture(varName, assetFile);
}



void FoamMaterial::SetTexture(const std::wstring varName, const std::wstring& assetFile) const
{
	TextureData* pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(varName))
	{
		pShaderVariable->AsShaderResource()->SetResource(pDiffuseTexture->GetShaderResourceView());
		return;
	}

	Logger::LogWarning(L"Shader variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}