#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
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

	// 4. Update Bones
	//Retrieve The Animator from the ModelComponent
	ModelAnimator* m_pAnimator = pModel->GetAnimator();
	//Make sure the animator is not NULL (ASSERT_NULL_)
	ASSERT_NULL_(m_pAnimator);
	//Retrieve the BoneTransforms from the Animator
	std::vector<XMFLOAT4X4> boneTransforms = m_pAnimator->GetBoneTransforms();
	//Set the 'gBones' variable of the effect (MatrixArray) > BoneTransforms
	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(boneTransforms.data()), (UINT)boneTransforms.size());
}
