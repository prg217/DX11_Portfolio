#include "pch.h"
#include "AE_Detail.h"

#include "AE_SpriteView.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CFlipBook.h>

AE_Detail::AE_Detail()
	: m_WidthSize(200)
	, m_CurSpriteIdx(0)
	, m_FPS(0)
	, m_AccTime(0)
{
}

AE_Detail::~AE_Detail()
{
	// new 해제
}

void AE_Detail::SetSprites(const vector<Ptr<CSprite>>& _Sprites)
{
	m_vecSprite = _Sprites;
}

void AE_Detail::Init()
{
}

void AE_Detail::Update()
{
	// FPS 설정
	ImGui::Text("FPS");
	ImGui::SameLine(100);
	ImGui::DragFloat("##FPS", &m_FPS);

	// 플립북 미리보기
	AniPreview();

	// 플립북 저장, 불러오기
	if (ImGui::Button("Save##FlipBook", ImVec2(50.f, 18.f)))
	{
		CFlipBook* pFlipBook = new CFlipBook;
		pFlipBook->SetSprites(m_vecSprite);

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"Animation\0*.flip";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"Animation\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			pFlipBook->Save(szSelect);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load##FlipBook", ImVec2(50.f, 18.f)))
	{
		CFlipBook* pFlipBook = new CFlipBook;

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"Animation\0*.flip";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"Animation\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			pFlipBook->Load(szSelect);
		}

		m_vecSprite = pFlipBook->GetSprites();
		OwnerSetSprites(m_vecSprite);
		GetSpriteView()->SetSprites(m_vecSprite);
	}
}

void AE_Detail::AniPreview()
{
	ImGui::Text("Preview");
	ImGui::SameLine(100);

	if (m_vecSprite.size() == 0)
	{
		return;
	}
	if (m_CurSpriteIdx >= m_vecSprite.size())
	{
		m_CurSpriteIdx = 0;
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

	// 실제 해상도 대비 출력 Image 의 비율
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
