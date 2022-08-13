#include "stdafx.h"
#include "ColorMaterial.h"

ColorMaterial::ColorMaterial():
	Material<ColorMaterial>(L"Effects/PosNorm3D.fx")
{}

void ColorMaterial::InitializeEffectVariables()
{
	UseTransparency(false);
}

void ColorMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"TransparencyTech" : L"Default");
}

void ColorMaterial::SetColor(const XMFLOAT4& color) const
{
	const std::wstring varName = L"gColor";
	SetVariable_Vector(varName, color);
}	
