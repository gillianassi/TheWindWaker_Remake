#include "stdafx.h"
#include "SkyboxMaterial.h"


SkyboxMaterial::SkyboxMaterial()
	: Material<SkyboxMaterial>(L"Effects/Skybox/Skybox_Replacement.fx")
{
}


void SkyboxMaterial::SetDiffuseTexture(const std::wstring& assetFile)
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

void SkyboxMaterial::InitializeEffectVariables()
{

}

