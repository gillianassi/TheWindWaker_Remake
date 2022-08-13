#include "stdafx.h"

#include "RupeeBlue.h"

RupeeBlue::RupeeBlue(Link* pLink):
	Rupee(pLink)
{}

void RupeeBlue::Initialize(const SceneContext& sceneContext)
{
	m_RupeeColor = XMFLOAT4{ 0.22f,0.39f,1.f,0.85f };
	m_RupeeValue = 5;

	Rupee::Initialize(sceneContext);
}

void RupeeBlue::Update(const SceneContext& sceneContext)
{
	Rupee::Update(sceneContext);
}

void RupeeBlue::DrawImGui()
{
	Rupee::DrawImGui();
}