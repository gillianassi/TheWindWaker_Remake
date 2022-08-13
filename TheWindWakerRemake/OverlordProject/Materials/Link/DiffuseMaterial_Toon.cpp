#include "stdafx.h"
#include "DiffuseMaterial_Toon.h"

DiffuseMaterial_Toon::DiffuseMaterial_Toon() :
	Material(L"Effects/Shadow/ToonShader.fx")
{}

void DiffuseMaterial_Toon::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}



void DiffuseMaterial_Toon::SetLightColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gLightColor";
	SetVariable_Vector(varName, color);
}

void DiffuseMaterial_Toon::SetDetail(float detail) const
{
	const std::wstring varName = L"gDetail";
	SetVariable_Scalar(varName, detail);
}

void DiffuseMaterial_Toon::SetStrength(float strength) const
{
	const std::wstring varName = L"gStrength";
	SetVariable_Scalar(varName, strength);
}

void DiffuseMaterial_Toon::SetBrightness(float brightness) const
{
	const std::wstring varName = L"gBrightness";
	SetVariable_Scalar(varName, brightness);
}


void DiffuseMaterial_Toon::InitializeEffectVariables()
{
	XMFLOAT4 lightColor = XMFLOAT4(1.f, 0.95f, 0.823f, 1.f);
	SetLightColor(lightColor);
	SetDetail(0.05f);
	SetBrightness(0.7f);
	SetStrength(0.7f);
}

void DiffuseMaterial_Toon::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	// 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	// LightWVP = model_world * light_viewprojection
	// (light_viewprojection[LightVP] can be acquired from the ShadowMapRenderer)
	DirectX::XMMATRIX lightVPMatrix = DirectX::XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP());
	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	DirectX::XMMATRIX LightWVPMatrix = worldMatrix * lightVPMatrix;
	DirectX::XMFLOAT4X4 LightWVP{};
	DirectX::XMStoreFloat4x4(&LightWVP, LightWVPMatrix);
	SetVariable_Matrix(L"gWorldViewProj_Light", LightWVP);
	
	// 2. Update the ShadowMap texture
	const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());
	
	// 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
}
