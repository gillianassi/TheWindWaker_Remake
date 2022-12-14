#include "stdafx.h"
#include "FontTestScene.h"

void FontTestScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.clearColor = XMFLOAT4{ 0.f,0.f,0.f,0.f };

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/WindWaker_64.fnt");
	m_pFont2 = ContentManager::Load<SpriteFont>(L"SpriteFonts/WindWaker_72.fnt");
}

void FontTestScene::Update()
{
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPosition, m_TextColor);
	TextRenderer::Get()->DrawText(m_pFont2, StringUtil::utf8_decode(m_Text), {}, XMFLOAT4(Colors::Yellow));
}

void FontTestScene::OnGUI()
{
	char buffer[256]{};
	m_Text.copy(&buffer[0], 256);
	if (ImGui::InputText("Text", &buffer[0], 256))
	{
		m_Text = std::string(buffer);
	}

	ImGui::SliderFloat2("Position", &m_TextPosition.x, 0, m_SceneContext.windowWidth);
	ImGui::ColorEdit4("Color", &m_TextColor.x, ImGuiColorEditFlags_NoInputs);
}
