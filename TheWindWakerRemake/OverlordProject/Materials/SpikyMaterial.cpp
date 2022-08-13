#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial():
	Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{
}

void SpikyMaterial::SetLightDirection(const XMFLOAT3& dir)
{
	const std::wstring varName = L"gLightDirection";
	SetVariable_Vector(varName, dir);
}

void SpikyMaterial::SetDiffuseColor(const XMFLOAT3& color) const
{
	const std::wstring varName = L"gColorDiffuse";
	SetVariable_Vector(varName, color);
}

void SpikyMaterial::SetSpikeLength(const float& length) const
{
	const std::wstring varName = L"gSpikeLength";
	SetVariable_Scalar(varName, length);
}

void SpikyMaterial::InitializeEffectVariables()
{

	SetLightDirection({ 0.551f,-0.534f,0.641f });
	SetSpikeLength(0.2f);
	SetDiffuseColor(XMFLOAT3{ 1,0,0 });
}
