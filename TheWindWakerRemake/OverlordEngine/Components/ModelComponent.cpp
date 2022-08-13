#include "stdafx.h"
#include "ModelComponent.h"

ModelComponent::ModelComponent(const std::wstring& assetFile, bool castShadows):
	m_AssetFile(assetFile),
	m_CastShadows(castShadows)
{
}

ModelComponent::~ModelComponent()
{
	SafeDelete(m_pAnimator);

	m_pDefaultMaterial = nullptr;
	m_Materials.clear();
}

void ModelComponent::Initialize(const SceneContext& sceneContext)
{
	m_pMeshFilter = ContentManager::Load<MeshFilter>(m_AssetFile);
	m_pMeshFilter->BuildIndexBuffer(sceneContext);

	//Resize Materials Array (if needed)
	if(m_Materials.size() < m_pMeshFilter->GetMeshCount())
	{
		m_Materials.resize(m_pMeshFilter->GetMeshCount(), nullptr);
	}

	if (m_pMeshFilter->m_HasAnimations)
		m_pAnimator = new ModelAnimator(m_pMeshFilter);

	if (m_MaterialChanged)
	{
		for(auto& subMesh: m_pMeshFilter->GetMeshes())
		{
			const auto pMaterial = m_Materials[subMesh.id]!=nullptr?m_Materials[subMesh.id]:m_pDefaultMaterial;
			m_pMeshFilter->BuildVertexBuffer(sceneContext, pMaterial, subMesh.id);
		}
		
		m_MaterialChanged = false;
	}

	if(m_CastShadows) //Only if we cast a shadow of course..
	{
		//1. Use ShadowMapRenderer::UpdateMeshFilter to update this MeshFilter for ShadowMap Rendering
		ShadowMapRenderer::Get()->UpdateMeshFilter(sceneContext, m_pMeshFilter);


		//2. Make sure to set m_enableShadowMapDraw to true, otherwise BaseComponent::ShadowMapDraw is not called
		m_enableShadowMapDraw = true;
	}
}

void ModelComponent::Update(const SceneContext& sceneContext)
{
	if (m_pAnimator)
		m_pAnimator->Update(sceneContext);
}

void ModelComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pDefaultMaterial)
	{
		Logger::LogWarning(L"ModelComponent::Draw() > No Default Material Set!");
		return;
	}

	// Update Submeshes
	for (const auto& subMesh : m_pMeshFilter->GetMeshes())
	{
		// do not draw the late submeshes yet
		if (IsLateSubmesh(&subMesh))continue;
		// otherwhise draw it already
		DrawSubmesh(&subMesh, sceneContext);
	}
	// Update late submeshes
	for (UINT8 subMeshID : m_LateSubmeshIDs)
	{
		DrawSubmesh(&(m_pMeshFilter->GetSubMesh(subMeshID)), sceneContext);
	}
}

bool ModelComponent::IsLateSubmesh(const SubMeshFilter* subMesh)
{
	for (UINT8 submeshID : m_LateSubmeshIDs)
	{
		if (subMesh->id == submeshID)
		{
			return true;
		}
	}
	return false;
}

void ModelComponent::DrawSubmesh(const SubMeshFilter* subMesh, const SceneContext& sceneContext)
{
	//Gather Material
	BaseMaterial* pCurrMaterial = nullptr;
	pCurrMaterial = m_Materials[subMesh->id] != nullptr ? m_Materials[subMesh->id] : m_pDefaultMaterial;
	pCurrMaterial->UpdateEffectVariables(sceneContext, this);

	const auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;

	//Set Inputlayout
	pDeviceContext->IASetInputLayout(pCurrMaterial->GetTechniqueContext().pInputLayout);

	//Set Vertex Buffer
	const UINT offset = 0;
	const auto& vertexBufferData = m_pMeshFilter->GetVertexBufferData(sceneContext, pCurrMaterial, subMesh->id);
	pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride,
		&offset);

	//Set Index Buffer
	pDeviceContext->IASetIndexBuffer(subMesh->buffers.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DRAW
	auto tech = pCurrMaterial->GetTechniqueContext().pTechnique;
	D3DX11_TECHNIQUE_DESC techDesc{};

	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(subMesh->indexCount, 0, 0);
	}
}

void ModelComponent::ShadowMapDraw(const SceneContext& sceneContext)
{
	//We only draw this Mesh to the ShadowMap if it casts shadows
	//This function is only called during the ShadowPass (and if m_enableShadowMapDraw is true)
	if (!m_CastShadows || !m_enableShadowMapDraw)return;

	//Here we want to Draw this Mesh to the ShadowMap, using the ShadowMapRenderer::DrawMesh function
	//1. Call ShadowMapRenderer::DrawMesh with the required function arguments BUT boneTransforms are only required for skinned meshes of course..
	if (m_pAnimator)
		ShadowMapRenderer::Get()->DrawMesh(sceneContext, m_pMeshFilter, GetTransform()->GetWorld(), m_pAnimator->GetBoneTransforms());
	else
		ShadowMapRenderer::Get()->DrawMesh(sceneContext, m_pMeshFilter, GetTransform()->GetWorld());
}

void ModelComponent::SetMaterial(BaseMaterial* pMaterial, UINT8 submeshId, bool isDrawnLate)
{
	//Resize Materials Array (if needed)
	if(m_Materials.size() <= submeshId)
	{
		m_Materials.resize(submeshId + 1, nullptr);
	}

	if (pMaterial == nullptr)
	{
		m_Materials[submeshId] = nullptr;
		return;
	}

	if (!pMaterial->HasValidMaterialId())
	{
		Logger::LogWarning(L"BaseMaterial does not have a valid BaseMaterial Id. Make sure to add the material to the material manager first.");
		return;
	}

	if(m_pDefaultMaterial == nullptr)
	{
		m_pDefaultMaterial = pMaterial;
	}

	m_Materials[submeshId] = pMaterial;
	m_MaterialChanged = true;
	if (isDrawnLate)
	{
		m_LateSubmeshIDs.push_back(submeshId);
	}


	if (m_IsInitialized && GetScene())
	{
		ASSERT_IF(m_pMeshFilter->GetMeshCount() <= submeshId, L"Invalid SubMeshID({}) for current MeshFilter({} submeshes)", submeshId, m_pMeshFilter->GetMeshCount())
		m_pMeshFilter->BuildVertexBuffer(GetScene()->GetSceneContext(), pMaterial, submeshId);
		m_MaterialChanged = false;
	}

}

void ModelComponent::SetMaterial(UINT materialId, UINT8 submeshId, bool isDrawnLate)
{
	const auto pMaterial = MaterialManager::Get()->GetMaterial(materialId);
	SetMaterial(pMaterial, submeshId, isDrawnLate);
}
