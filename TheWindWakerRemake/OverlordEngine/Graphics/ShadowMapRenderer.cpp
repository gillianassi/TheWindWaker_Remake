#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "Misc/ShadowMapMaterial.h"

ShadowMapRenderer::~ShadowMapRenderer()
{
	SafeDelete(m_pShadowRenderTarget)
}

void ShadowMapRenderer::Initialize()
{
	if (m_IsInitialized)
		return;
	//1. Create a separate RenderTarget (see RenderTarget class), store in m_pShadowRenderTarget
	//	- RenderTargets are initialized with the RenderTarget::Create function, requiring a RENDERTARGET_DESC
	//	- Initialize the relevant fields of the RENDERTARGET_DESC (enableColorBuffer:false, enableDepthSRV:true, width & height)
	m_pShadowRenderTarget = new RenderTarget(m_GameContext.d3dContext);
	RENDERTARGET_DESC renderTargetDesc{};
	renderTargetDesc.enableDepthBuffer = true;
	renderTargetDesc.enableDepthSRV = true;
	renderTargetDesc.enableColorBuffer = false;
	renderTargetDesc.enableColorSRV = false;
	renderTargetDesc.generateMipMaps_Color = false;
	renderTargetDesc.depthFormat = DXGI_FORMAT_D32_FLOAT;
	renderTargetDesc.width = 1280;
	renderTargetDesc.height = 720;
	m_pShadowRenderTarget->Create(renderTargetDesc);

	//2. Create a new ShadowMapMaterial, this will be the material that 'generated' the ShadowMap, store in m_pShadowMapGenerator
	//	- The effect has two techniques, one for static meshes, and another for skinned meshes (both very similar, the skinned version also transforms the vertices based on a given set of boneTransforms)
	//	- We want to store the TechniqueContext (struct that contains information about the Technique & InputLayout required for rendering) for both techniques in the m_GeneratorTechniqueContexts array.
	//	- Use the ShadowGeneratorType enum to retrieve the correct TechniqueContext by ID, and also use that ID to store it inside the array (see BaseMaterial::GetTechniqueContext)
	
	m_pShadowMapGenerator = MaterialManager::Get()->CreateMaterial< ShadowMapMaterial>();
	int index = (int)ShadowGeneratorType::Static;
	m_GeneratorTechniqueContexts[index] = m_pShadowMapGenerator->GetTechniqueContext(index);
	index = (int)ShadowGeneratorType::Skinned;
	m_GeneratorTechniqueContexts[index] = m_pShadowMapGenerator->GetTechniqueContext(index);

	m_IsInitialized = true;
}

void ShadowMapRenderer::UpdateMeshFilter(const SceneContext& sceneContext, MeshFilter* pMeshFilter) const
{
	//Here we want to Update the MeshFilter of ModelComponents that need to be rendered to the ShadowMap
	//Updating the MeshFilter means that we want to create a corresponding VertexBuffer for our ShadowGenerator material
	MaterialTechniqueContext currentTechnique;
	//1. Figure out the correct ShadowGeneratorType (either Static, or Skinned) with information from the incoming MeshFilter
	//if (pMeshFilter->HasAnimations())
	//{
	//	//2. Retrieve the corresponding TechniqueContext from m_GeneratorTechniqueContexts array (Static/Skinned)
	//	currentTechnique = m_GeneratorTechniqueContexts[(int)ShadowGeneratorType::Skinned];
	//}
	//else
	//{
	//	currentTechnique = m_GeneratorTechniqueContexts[(int)ShadowGeneratorType::Static];
	//}
	//1. Figure out the correct ShadowGeneratorType (either Static, or Skinned) with information from the incoming MeshFilter
	//2. Retrieve the corresponding TechniqueContext from m_GeneratorTechniqueContexts array (Static/Skinned)
	// Static is 0 -> animations = false = 0
	// Skinned is 1 -> animations = true = 1
	currentTechnique = m_GeneratorTechniqueContexts[(int)pMeshFilter->HasAnimations()];
	//3. Build a corresponding VertexBuffer with data from the relevant TechniqueContext you retrieved in Step2 (MeshFilter::BuildVertexBuffer)
	pMeshFilter->BuildVertexBuffer(sceneContext, currentTechnique.inputLayoutID, currentTechnique.inputLayoutSize, currentTechnique.pInputLayoutDescriptions);
	
	
}

void ShadowMapRenderer::Begin(const SceneContext& sceneContext)
{
	//This function is called once right before we start the Shadow Pass (= generating the ShadowMap)
	//This function is responsible for setting the pipeline into the correct state, meaning
	//	- Making sure the ShadowMap is unbound from the pipeline as a ShaderResource (SRV), so we can bind it as a RenderTarget (RTV)
	//	- Calculating the Light ViewProjection, and updating the relevant Shader variables
	//	- Binding the ShadowMap RenderTarget as Main Game RenderTarget (= Everything we render is rendered to this rendertarget)
	//	- Clear the current (which should be the ShadowMap RT) rendertarget

	//1. Making sure that the ShadowMap is unbound from the pipeline as ShaderResourceView (SRV) is 
	// important, because we cannot use the same resource as a ShaderResourceView (texture resource inside a shader) 
	// and a RenderTargetView (target everything is rendered too) at the same time. 
	// In case this happens, you'll see an error in the output of visual studio -
	//  warning you that a resource is still bound as a SRV and cannot be used as an RTV.
	//	-> Unbinding an SRV can be achieved using DeviceContext::PSSetShaderResource [I'll give you the implementation for free] - 
	// double check your output because depending on your usage of ShaderResources, the actual slot the ShadowMap is using can be 
	// different, but you'll see a warning pop-up with the correct slot ID in that case.
	constexpr ID3D11ShaderResourceView* const pSRV[] = { nullptr };
	sceneContext.d3dContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);

	//2. Calculate the Light ViewProjection and store in m_LightVP
	// - Use XMMatrixOrtographicLH to create Projection Matrix (constants used for the demo below - feel free to change)

	float viewWidth = 1.f * sceneContext.aspectRatio;
	float viewHeight = 1.0f;
	float nearZ = 0.1f;
	float farZ = 10.0f;
	auto projection = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);

	//- Use XMMatrixLookAtLH to create a View Matrix
	//		*eyePosition: Position of the Direction Light (SceneContext::pLights > Retrieve Directional Light)
	//		*focusPosition: Calculate using the Direction Light position and direction
	auto eyePosition = XMLoadFloat4(&sceneContext.pLights->GetDirectionalLight().position);
	auto direction = XMLoadFloat4(&sceneContext.pLights->GetDirectionalLight().direction);
	XMVECTOR upVec = XMVectorSet( 0,1,0, 0 );
	auto view = XMMatrixLookAtLH(eyePosition, eyePosition + direction, upVec);


	//- Use the Projection & View Matrix to calculate the ViewProjection of this Light, store in m_LightVP
	XMMATRIX viewproj = view * projection;
	XMStoreFloat4x4(&m_LightVP, viewproj);
	
	//3. Update this matrix (m_LightVP) on the ShadowMapMaterial effect
	m_pShadowMapGenerator->SetVariable_Matrix(L"gLightViewProj", m_LightVP);

	//4. Set the Main Game RenderTarget to m_pShadowRenderTarget (OverlordGame::SetRenderTarget) - Hint: every Singleton object has access to the GameContext...
	m_GameContext.pGame->SetRenderTarget(m_pShadowRenderTarget);

	//5. Clear the ShadowMap rendertarget (RenderTarget::Clear)
	m_pShadowRenderTarget->Clear();
}

void ShadowMapRenderer::DrawMesh(const SceneContext& sceneContext, MeshFilter* pMeshFilter, const XMFLOAT4X4& meshWorld, const std::vector<XMFLOAT4X4>& meshBones)
{
	//This function is called for every mesh that needs to be rendered on the shadowmap (= cast shadows)

	//1. Figure out the correct ShadowGeneratorType (Static or Skinned)
	//2. Retrieve the correct TechniqueContext for m_GeneratorTechniqueContexts
	bool hasAnimations = !meshBones.empty();
	MaterialTechniqueContext currentTechnique = m_GeneratorTechniqueContexts[(int)hasAnimations];
	//3. Set the relevant variables on the ShadowMapMaterial
	//		- world of the mesh
	//		- if animated, the boneTransforms
	m_pShadowMapGenerator->SetVariable_Matrix(L"gWorld", meshWorld);


	if (hasAnimations)
		m_pShadowMapGenerator->SetVariable_MatrixArray(L"gBones", &meshBones[0]._11, (UINT)meshBones.size());

	//4. Setup Pipeline for Drawing (Similar to ModelComponent::Draw, but for our ShadowMapMaterial)

    // - Set correct TechniqueContext on ShadowMapMaterial - use ShadowGeneratorType as ID (BaseMaterial::SetTechnique)
	m_pShadowMapGenerator->SetTechnique((int)hasAnimations);

	const auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	// - Set InputLayout (see TechniqueContext)
	pDeviceContext->IASetInputLayout(m_pShadowMapGenerator->GetTechniqueContext().pInputLayout);

	// - Set PrimitiveTopology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	- Iterate the SubMeshes of the MeshFilter (see ModelComponent::Draw), for each SubMesh:
	for (const auto& subMesh : pMeshFilter->GetMeshes())
	{

		// - Set VertexBuffer

		const UINT offset = 0;
		const auto& vertexBufferData = pMeshFilter->GetVertexBufferData(sceneContext, m_pShadowMapGenerator, subMesh.id);
		pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride,
			&offset);

		// - Set IndexBuffer
		pDeviceContext->IASetIndexBuffer(subMesh.buffers.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// - Perform Draw Call (same as usual, iterate Technique Passes, Apply, Draw - See ModelComponent::Draw for reference)
		auto tech = m_pShadowMapGenerator->GetTechniqueContext().pTechnique;
		D3DX11_TECHNIQUE_DESC techDesc{};

		tech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			tech->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(subMesh.indexCount, 0, 0);
		}
	}

}

void ShadowMapRenderer::End(const SceneContext&) const
{
	// This function is called at the end of the Shadow-pass, all shadow-casting meshes should be drawn to the ShadowMap at this point.
	// Now we can reset the Main Game Rendertarget back to the original RenderTarget, this also Unbinds the ShadowMapRenderTarget as 
	// RTV from the Pipeline, and can safely use it as a ShaderResourceView after this point.

	//1. Reset the Main Game RenderTarget back to default (OverlordGame::SetRenderTarget)
	m_GameContext.pGame->SetRenderTarget(nullptr);
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowMap() const
{
	return m_pShadowRenderTarget->GetDepthShaderResourceView();
}

void ShadowMapRenderer::Debug_DrawDepthSRV(const XMFLOAT2& position, const XMFLOAT2& scale, const XMFLOAT2& pivot) const
{
	if (m_pShadowRenderTarget->HasDepthSRV())
	{
		SpriteRenderer::Get()->DrawImmediate(m_GameContext.d3dContext, m_pShadowRenderTarget->GetDepthShaderResourceView(), position, XMFLOAT4{ Colors::White }, pivot, scale);

		//Remove from Pipeline
		constexpr ID3D11ShaderResourceView* const pSRV[] = { nullptr };
		m_GameContext.d3dContext.pDeviceContext->PSSetShaderResources(0, 1, pSRV);
	}
}
