#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	delete m_ParticlesArray;
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	// m_pParticleMaterial is static and shared across all ParticleEmitterComponents
	if (!m_pParticleMaterial)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}

	CreateVertexBuffer(sceneContext);
	
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	// Release the vertex buffer if it already exists
	if (m_pVertexBuffer)
	{
		SafeRelease(m_pVertexBuffer);
	}
	//Create a dynamic vertexbuffer
	D3D11_BUFFER_DESC vertexBufferDescr;
	//	a.Set the usage to Dynamic
	vertexBufferDescr.Usage = D3D11_USAGE_DYNAMIC;
	//	b.Use the ParticleCountand size of a VertexParticle to calculate the ByteWidth
	vertexBufferDescr.ByteWidth = sizeof(VertexParticle) * m_ParticleCount;
	//	c.This is a VertexBuffer, select the appropriate BindFlag
	vertexBufferDescr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	d.We want to edit the buffer at - runtime, so we need CPU write access.Select the appropriate CPUAccessFlag
	vertexBufferDescr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	e.There are no MiscFlags(0)
	vertexBufferDescr.MiscFlags = 0;
	//	f.Create the VertexBuffer(GraphicsDevice > …)
	HRESULT hr = sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBufferDescr, nullptr, &m_pVertexBuffer);
	//	g.Don’t forget to check for errors!(HANDLE_ERROR)
	HANDLE_ERROR(hr);
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	// Create local variable containing the average particle emit treshold
	// This is calculated using the average energy of the particle (maxEnergy vs minEnergy)
	float particleInterval = ((m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / 2) / m_ParticleCount;

	// Cashe the elapsed time for later use
	float elapsedTime = sceneContext.pGameTime->GetElapsed();
	// increase the lastParticleInit by the elapsed Game Time
	m_LastParticleSpawn += elapsedTime;

	// validate particles and add some to the vertexBuffer
	// set activer particles to zero
	m_ActiveParticles = 0;

	// Map our vertexbuffer (DeviceContext::Map), to retrieve an element (type D3D11_MAPPED_SUBRESOURCE)
	// which contains a pointer to the first element in our vertexbuffer. 
	// Between the ‘Map’ and ‘Unmap’ method we can update the VertexBuffer data.
	// a. override the urrent data in the vertex buffer to discard the previous data (D3D11_MAP_WRITE_DISCARD)
	// b. Give it no mapflags
	// c. insert the mapped resource as the final parameter
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	
	// use the mapped resource to return the pointer to the first element of the paticle vertex (VertexParticle)
	VertexParticle* pBuffer = static_cast<VertexParticle*>(mappedResource.pData);
	
	bool hasSpawned = false;
	// Now the buffer is mapped and can be used to write data to
	// Itterate over every particle
	for (size_t i = 0; i < m_ParticleCount; i++)
	{
		Particle& particle = m_ParticlesArray[i];
		// update the particle when it's active
		if (particle.isActive)
		{
			UpdateParticle(particle, elapsedTime);
		}
		// repeat the particle.isActive call, because the particle can become inactive during the update cycle
		if (!hasSpawned && m_isActive && !particle.isActive && (m_LastParticleSpawn >= particleInterval))
		{
			SpawnParticle(particle);
			hasSpawned = true;
		}
		if (m_ParticlesArray[i].isActive)
		{
			// Add all active particles to the vertexBuffer
			// Note: after the spawn we can assume thatt the particle will be active
			pBuffer[m_ActiveParticles] = particle.vertexInfo;
			++m_ActiveParticles;
		}

	}
	// Use DeviceContext::Unmap to unmap our vertexbuffer.
	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);

}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{

	// Subtract the elapsedTime from the particle’s currentEnergy.
	p.currentEnergy -= elapsedTime;

	// deactivate the particle and return if the current energy <= 0;
	if (p.currentEnergy <= 0)
	{
		p.isActive = false;
		return;
	}
	
	// update all the variables of our VertexParticle parameter
	// Move the particle in the direction of the velocity defined by the emitter settings (m_EmitterSettings)
	p.vertexInfo.Position.x += m_EmitterSettings.velocity.x * elapsedTime;
	p.vertexInfo.Position.y += m_EmitterSettings.velocity.y * elapsedTime;
	p.vertexInfo.Position.z += m_EmitterSettings.velocity.z * elapsedTime;
	
	// represent the percentual lifetime -> It slowly increases
	float lifePercent = p.currentEnergy / p.totalEnergy;
	// Inverse Life Percent can be seen as the energy depleting
	float inverseLifePercent = 1.f - lifePercent;

	// Use the color given by the emittersettings
	// delete this line if you want to gradUally change colors
	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w = p.vertexInfo.Color.w * lifePercent * 2.f; // *2 is to delay the fade out effect

	// Based on the particle’s sizeChange value, our particle shrinks or grows over time.
	// weighted interpolation FROM initial size TO "size * sizechange"
	p.vertexInfo.Size = p.initialSize * (lifePercent + p.sizeChange * inverseLifePercent);


}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	// make the particle active
	//----------------------
	p.isActive = true;
	
	// Initialize the energy
	//----------------------
	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	// Initialize the position
	//----------------------
	// Calculate a random direction 
	// Create a unit vector
	DirectX::XMVECTOR randomDirection{ 1, 0, 0 };
	// create a random rotation matrix
	auto randomRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
		MathHelper::randF(-DirectX::XM_PI, DirectX::XM_PI), 
		MathHelper::randF(-DirectX::XM_PI, DirectX::XM_PI), 
		MathHelper::randF(-DirectX::XM_PI, DirectX::XM_PI));
	// Use XMVector3TransformNormal to transfer the rotation to the unit vector
	auto randomNormalizedVector = DirectX::XMVector3TransformNormal(randomDirection, randomRotationMatrix);
	// Calculate the distance (radius) away from our starting point
	float distance = MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius);
	XMVECTOR transformPos = DirectX::XMLoadFloat3(&GetTransform()->GetPosition());

	// Calculate the initial position
	DirectX::XMStoreFloat3(&p.vertexInfo.Position, transformPos + randomNormalizedVector * distance);

	// Initialize the Size and scale
	//----------------------
	p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.vertexInfo.Size = p.initialSize;
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	// Initialize the vertex rotation between [-Pi, Pi]
	//----------------------
	p.vertexInfo.Rotation = MathHelper::randF(-DirectX::XM_PI, DirectX::XM_PI);
	
	// Initialize the color
	//----------------------
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	// Note: This is not the same as "Draw"!!
	// Note: that the particles are already in world space -> No transform needed!
	ID3D11DeviceContext* pDeviceContext = sceneContext.d3dContext.pDeviceContext;

	// Set the material variables
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj",  &sceneContext.pCamera->GetViewProjection()._11);
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse",  &sceneContext.pCamera->GetViewProjectionInverse()._11);
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture->GetShaderResourceView());

	// Retrieve the TechniqueContext from the material, 
	// this structure contains relevant information to setup the pipeline (BaseMaterial::GetTechniqueContext)
	MaterialTechniqueContext techniqueContext = m_pParticleMaterial->GetTechniqueContext();
	D3DX11_TECHNIQUE_DESC techniqueDesc;
	techniqueContext.pTechnique->GetDesc(&techniqueDesc);

	// Set the InputLayout
	pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);
	
	// Set the primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	// Set vertexBuffer
	UINT offset = 0;
	const UINT stridesSize = sizeof(VertexParticle);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stridesSize, &offset);

	// Apply the pass for each pass of our technique
	for (uint32_t i = 0; i < techniqueDesc.Passes; i++)
	{
		techniqueContext.pTechnique->GetPassByIndex(i)->Apply(0, pDeviceContext);
		// Draw the vertices!(The number of vertices we want to draw = m_ActiveParticles)
		pDeviceContext->Draw(m_ActiveParticles, 0);
	}

}

void ParticleEmitterComponent::DrawImGui()
{
	
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::Checkbox("IsActive", &m_isActive);
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}