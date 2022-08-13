#include "stdafx.h"
#include "EyeMaterial.h"

EyeMaterial::EyeMaterial() :
	Material<EyeMaterial>(L"Effects/Link/EyeShader_Skinned.fx")
{}

void EyeMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void EyeMaterial::InitializeEffectVariables()
{
}

void EyeMaterial::OnUpdateModelVariables(const SceneContext&, const ModelComponent* pModel) const
{
	//Retrieve The Animator from the ModelComponent
	ModelAnimator* m_pAnimator = pModel->GetAnimator();
	//Make sure the animator is not NULL (ASSERT_NULL_)
	ASSERT_NULL_(m_pAnimator);
	//Retrieve the BoneTransforms from the Animator
	std::vector<XMFLOAT4X4> boneTransforms = m_pAnimator->GetBoneTransforms();
	//Set the 'gBones' variable of the effect (MatrixArray) > BoneTransforms
	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(boneTransforms.data()), (UINT)boneTransforms.size());


}

