#include "stdafx.h"

#include "RupeeYellow.h"

RupeeYellow::RupeeYellow(Link* pLink) :
	Rupee(pLink)
{}

void RupeeYellow::Initialize(const SceneContext& sceneContext)
{
	m_RupeeColor = XMFLOAT4{ 1.f, 0.918f, 0.3f,0.85f };
	m_RupeeValue = 10;

	Rupee::Initialize(sceneContext);
}

void RupeeYellow::Update(const SceneContext& sceneContext)
{
	Rupee::Update(sceneContext);
}

void RupeeYellow::DrawImGui()
{
	Rupee::DrawImGui();
}