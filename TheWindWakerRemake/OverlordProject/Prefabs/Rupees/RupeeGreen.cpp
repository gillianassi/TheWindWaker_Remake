#include "stdafx.h"

#include "RupeeGreen.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/Link.h"

RupeeGreen::RupeeGreen(Link* pLink):
	Rupee(pLink)
{}

void RupeeGreen::Initialize(const SceneContext& sceneContext)
{
	m_RupeeColor = XMFLOAT4{ 0.23f,1.f,0.f,0.85f };
	m_RupeeValue = 1;

	Rupee::Initialize(sceneContext);
}

void RupeeGreen::Update(const SceneContext& sceneContext)
{
	Rupee::Update(sceneContext);
}

void RupeeGreen::DrawImGui()
{
	Rupee::DrawImGui();
}