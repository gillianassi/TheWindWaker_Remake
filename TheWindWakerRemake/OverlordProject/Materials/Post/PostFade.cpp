//Resharper Disable All

	#include "stdafx.h"
	#include "PostFade.h"

	PostFade::PostFade():
		PostProcessingMaterial(L"Effects/Post/Fade.fx")
	{
	}

	void PostFade::SetFadeWeight(float weight) const
	{
		m_pBaseEffect->GetVariableByName("gFadeWeight")->AsScalar()->SetFloat(weight);
	}



