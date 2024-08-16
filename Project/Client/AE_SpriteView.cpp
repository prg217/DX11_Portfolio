#include "pch.h"
#include "AE_SpriteView.h"

void AE_SpriteView::Init()
{
}

void AE_SpriteView::Update()
{
	// 이미지 for문으로 추가한 만큼 넣기
	//GetSprites();
	//for ()
	//ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), m_AtlasTex->Height() * m_Ratio), uv_min, uv_max, tint_col, border_col);
}

void AE_SpriteView::SetSprites(vector<Ptr<CSprite>> _Sprites)
{
	//if (m_vecSprite == _Sprites)
		//return;

	//m_vecSprite = _Sprites;
}

AE_SpriteView::AE_SpriteView()
{
}

AE_SpriteView::~AE_SpriteView()
{
}
