#include "stdafx.h"
#include "EdgeFoamMaterial.h"

EdgeFoamMaterial::EdgeFoamMaterial() :
	Material<EdgeFoamMaterial>(L"Effects/Water/EdgeFoam.fx")
{}

void EdgeFoamMaterial::InitializeEffectVariables()
{
	SetUVSpeed(XMFLOAT2{ 0.3f, 0.f });
	SetVSquish(2.f);
	SetFoamTexture(L"Textures/Sea/Shore.png");
}

void EdgeFoamMaterial::SetFoamColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gFoamColor";
	SetVariable_Vector(varName, color);
}


void EdgeFoamMaterial::SetVSquish(float squish) const
{
	const std::wstring varName = L"gVSquish";
	SetVariable_Scalar(varName, squish);
}
void EdgeFoamMaterial::SetDepthBias(float bias) const
{
	const std::wstring varName = L"gDepthBias";
	SetVariable_Scalar(varName, bias);
}

void EdgeFoamMaterial::SetUVSpeed(const XMFLOAT2& uvSpeed) const
{
	const XMFLOAT3& convert = XMFLOAT3(uvSpeed.x, uvSpeed.y, 0.f);
	const std::wstring varName = L"gUVSpeed";
	SetVariable_Vector(varName, convert);
}

void EdgeFoamMaterial::SetTotalTime(float totalTime) const
{
	const std::wstring varName = L"gTotalTime";
	SetVariable_Scalar(varName, totalTime);
}


void EdgeFoamMaterial::SetFoamTexture(const std::wstring& assetFile) const
{
	const std::wstring varName = L"gFoamDiffuse";
	SetTexture(varName, assetFile);
}



void EdgeFoamMaterial::SetTexture(const std::wstring varName, const std::wstring& assetFile) const
{
	TextureData* pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	if (const auto pShaderVariable = GetVariable(varName))
	{
		pShaderVariable->AsShaderResource()->SetResource(pDiffuseTexture->GetShaderResourceView());
		return;
	}

	Logger::LogWarning(L"Shader variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}