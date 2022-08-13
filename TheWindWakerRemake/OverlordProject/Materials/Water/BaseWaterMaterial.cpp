#include "stdafx.h"
#include "BaseWaterMaterial.h"

BaseWaterMaterial::BaseWaterMaterial() :
	Material<BaseWaterMaterial>(L"Effects/Water/BaseWater.fx")
{}

void BaseWaterMaterial::InitializeEffectVariables()
{
	UseTransparency(false);
}

void BaseWaterMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"TransparencyTech" : L"Default");
}

void BaseWaterMaterial::SetColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gColor";
	SetVariable_Vector(varName, color);
}	
