#include "pch.h"
#include "SpriteComUI.h"

#include <Engine/CSpriteComponent.h>

SpriteComUI::SpriteComUI()
	: ComponentUI(COMPONENT_TYPE::SPRITECOMPONENT)
{
}

SpriteComUI::~SpriteComUI()
{
}

void SpriteComUI::Update()
{
	Title();

	// ÀÌ¹ÌÁö
	CSpriteComponent* spriteCom = GetTargetObject()->SpriteComponent();

	Ptr<CSprite> pSprite = spriteCom->GetSprite();
	Ptr<CTexture> pTex = pSprite->GetAtlasTexture();

	Vec2 tempLeftTopUV = pSprite->GetLeftTopUV();
	ImVec2 leftTopUV = ImVec2(tempLeftTopUV.x, tempLeftTopUV.y);

	Vec2 tempSliceUV = pSprite->GetSliceUV();
	Vec2 tempRightDownUV = tempLeftTopUV + tempSliceUV;
	tempRightDownUV -= pSprite->GetOffsetUV();
	ImVec2 rightDownUV = ImVec2(tempRightDownUV.x, tempRightDownUV.y);

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	float widthSize = 200.f;

	if (pSprite == nullptr)
	{
		ImGui::Image(nullptr, ImVec2(widthSize, widthSize), uv_min, uv_max, tint_col, border_col);
	}
	else
	{
		float ratio = widthSize / pTex->Width();
		ImGui::Image(pTex->GetSRV().Get(), ImVec2(widthSize, pTex->Height() * ratio), leftTopUV, rightDownUV, tint_col, border_col);
	}
}

