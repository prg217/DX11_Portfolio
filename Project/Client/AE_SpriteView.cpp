#include "pch.h"
#include "AE_SpriteView.h"

AE_SpriteView::AE_SpriteView()
	: m_WidthSize(200)
{
}

AE_SpriteView::~AE_SpriteView()
{
}

void AE_SpriteView::Init()
{
}

void AE_SpriteView::Update()
{
	if (m_vecSprite.size() == 0)
	{
		return;
	}

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	for (auto i : m_vecSprite)
	{
		Ptr<CTexture> tex = i->GetAtlasTexture();

		Vec2 tempLeftTopUV = i->GetLeftTopUV();
		ImVec2 leftTopUV = ImVec2(tempLeftTopUV.x, tempLeftTopUV.y);

		Vec2 tempSliceUV = i->GetSliceUV();
		Vec2 tempRightDownUV = tempLeftTopUV + tempSliceUV;
		tempRightDownUV -= i->GetOffsetUV();
		ImVec2 rightDownUV = ImVec2(tempRightDownUV.x, tempRightDownUV.y);

		// ���� �ػ� ��� ��� Image �� ����
		float ratio = m_WidthSize / tex->Width();

		// _in.vUV : �������̸� ������ ��ġ�� ������ ȯ���� ��
		//Vec2 tempBGUV = i->GetBackgroundUV();
		//Vec2 BackGroundLeftTop = tempLeftTopUV - (tempBGUV - tempSliceUV) / Vec2(2.f, 2.f);
		//Vec2 vSpriteUV = BackGroundLeftTop + ( 0.7 * i->GetBackgroundUV());
		//vSpriteUV -= i->GetOffsetUV();

		//ImVec2 BGLeftTop = ImVec2(BackGroundLeftTop.x, BackGroundLeftTop.y);
		//ImVec2 spriteUV = ImVec2(vSpriteUV.x, vSpriteUV.y);

		ImGui::Image(tex->GetSRV().Get(), ImVec2((m_WidthSize), tex->Height() * ratio), leftTopUV, rightDownUV, tint_col, border_col);

		// ������ ��������Ʈ�� �ƴ϶�� ���� ���ο� �ְ� ���ش�.
		if (i != m_vecSprite[m_vecSprite.size() - 1])
		{
			ImGui::SameLine();
		}
	}
}

void AE_SpriteView::SetSprites(vector<Ptr<CSprite>>  _Sprites)
{
	m_vecSprite = _Sprites;
}