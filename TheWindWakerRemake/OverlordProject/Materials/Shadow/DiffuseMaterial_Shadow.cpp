#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow.fx")
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}


void DiffuseMaterial_Shadow::SetBrightness(float brightness) const
{
	const std::wstring varName = L"gBrightness";
	SetVariable_Scalar(varName, brightness);
}


void DiffuseMaterial_Shadow::InitializeEffectVariables()
{

	SetBrightness(0.085f);
}

void DiffuseMaterial_Shadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
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
