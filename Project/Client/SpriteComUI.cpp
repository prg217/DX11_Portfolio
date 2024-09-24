#include "pch.h"
#include "SpriteComUI.h"

#include <Engine/CSpriteComponent.h>
#include <Engine/CAssetMgr.h>

#include "ListUI.h"
#include "TreeUI.h"
#include "CEditorMgr.h"

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

	CSpriteComponent* spriteCom = GetTargetObject()->SpriteComponent();

	if (GetTargetObject()->IsDead())
	{
		return;
	}

	// ===========이미지===========
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

	if (ImGui::Button("##SpriteBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Sprite");
		vector<string> vecSpriteNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
		pListUI->AddList(vecSpriteNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&SpriteComUI::SelectSprite);
		pListUI->SetActive(true);
	}
	// ============================

	// 색 적용
	ImGui::Text("AddColor");
	ImGui::SameLine(100);
	bool addColor = spriteCom->GetAddColor();
	ImGui::Checkbox("##SpriteAddColor", &addColor);

	Vec3 Color = spriteCom->GetColor();

	if (addColor)
	{
		ImGui::Text("Color");
		ImGui::SameLine(100);
		ImGui::ColorEdit3("##SpriteColor", Color);
	}

	spriteCom->AddColor(addColor, Color);

	// 빛 적용 유무
	ImGui::Text("UseLight");
	ImGui::SameLine(100);
	bool useLight = spriteCom->GetUseLight();
	if (ImGui::Checkbox("##SpriteUseLight", &useLight))
	{
		spriteCom->SetUseLight(useLight);
	}
}

void SpriteComUI::SelectSprite(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(strAssetName);

	assert(pSprite.Get());

	CSpriteComponent* spriteCom = GetTargetObject()->SpriteComponent();
	spriteCom->AddSprite(pSprite);
}

