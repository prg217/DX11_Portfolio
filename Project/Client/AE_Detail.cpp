#include "pch.h"
#include "AE_Detail.h"

#include <Engine/CTimeMgr.h>

AE_Detail::AE_Detail()
	: m_WidthSize(200)
	, m_CurSpriteIdx(0)
	, m_FPS(0)
	, m_AccTime(0)
{
}

AE_Detail::~AE_Detail()
{
}

void AE_Detail::SetSprites(vector<Ptr<CSprite>> _Sprites)
{
	m_vecSprite = _Sprites;
}

void AE_Detail::Init()
{
}

void AE_Detail::Update()
{
	// FPS ����
	ImGui::Text("FPS");
	ImGui::SameLine(100);
	ImGui::DragFloat("##FPS", &m_FPS);

	// �ø��� �̸�����
	AniPreview();

	// �ø��� ����, �ҷ�����

}

void AE_Detail::AniPreview()
{
	ImGui::Text("Preview");
	ImGui::SameLine(100);

	if (m_vecSprite.size() == 0)
	{
		return;
	}

	float MaxTime = 1.f / m_FPS;

	m_AccTime += EngineDT;

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	Ptr<CSprite> curSprite = m_vecSprite[m_CurSpriteIdx];
	Ptr<CTexture> tex = curSprite->GetAtlasTexture();

	Vec2 tempLeftTopUV = curSprite->GetLeftTopUV();
	ImVec2 leftTopUV = ImVec2(tempLeftTopUV.x, tempLeftTopUV.y);

	Vec2 tempSliceUV = curSprite->GetSliceUV();
	Vec2 tempRightDownUV = tempLeftTopUV + tempSliceUV;
	tempRightDownUV -= curSprite->GetOffsetUV();
	ImVec2 rightDownUV = ImVec2(tempRightDownUV.x, tempRightDownUV.y);

	// ���� �ػ� ��� ��� Image �� ����
	float ratio = m_WidthSize / tex->Width();

	ImGui::Image(tex->GetSRV().Get(), ImVec2((m_WidthSize), tex->Height() * ratio), leftTopUV, rightDownUV, tint_col, border_col);

	if (MaxTime < m_AccTime)
	{
		m_AccTime -= MaxTime;

		m_CurSpriteIdx++;
		if (m_CurSpriteIdx >= m_vecSprite.size())
		{
			m_CurSpriteIdx = 0;
		}
	}
}
