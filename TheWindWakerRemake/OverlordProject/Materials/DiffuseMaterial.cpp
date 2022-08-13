#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial() :
	Material<DiffuseMaterial>(L"Effects/PosNormTex3d.fx")
{}

void DiffuseMaterial::InitializeEffectVariables()
{
	UseTransparency(false);
}

void DiffuseMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"TransparencyTech" : L"Default");
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	const std::wstring varName = L"gDiffuseMap";
	if (const auto pShaderVariable = GetVariable(varName))
	{
		pShaderVariable->AsShaderResource()->SetResource(m_pDiffuseTexture->GetShaderResourceView());
		return;
	}

	Logger::LogWarning(L"Shader variable \'{}\' not found for \'{}\'", varName, GetEffectName());
}

