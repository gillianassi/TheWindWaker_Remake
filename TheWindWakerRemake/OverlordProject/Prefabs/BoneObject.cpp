#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length):
	m_Length{length},
	m_pMaterial{pMaterial},
	m_BindPose{}
{
}


void BoneObject::Initialize(const SceneContext&)
{
	// Create an empty Game object
	GameObject* pEmpty = new GameObject{};
	// Add it as a child of this bone
	this->AddChild(pEmpty);
	// Add a model component to that object
	ModelComponent* pModel = pEmpty->AddComponent(new ModelComponent(L"Meshes/Bone.ovm"));
	// Assign the bone material to the model
	pModel->SetMaterial(m_pMaterial);
	// Rotate the gameobject 90 degrees around the y axis (orient along x-axis)
	pModel->GetTransform()->Rotate(0, -90, 0);
	// scale the game object to match the bone length
	pModel->GetTransform()->Scale(m_Length, m_Length, m_Length);
}

void BoneObject::CalculateBindPose()
{
	XMFLOAT4X4 worldMatrix = this->GetTransform()->GetWorld();
	XMMATRIX inverseWorldMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&worldMatrix));
	XMStoreFloat4x4(&m_BindPose, inverseWorldMatrix);

	for (BoneObject* boneChild : GetChildren<BoneObject>())
	{
		boneChild->CalculateBindPose();
	}
}
void BoneObject::AddChildBone(BoneObject* pBone)
{
	// translate along the X-axis -> next to the parent bone
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	this->AddChild(pBone);
}
